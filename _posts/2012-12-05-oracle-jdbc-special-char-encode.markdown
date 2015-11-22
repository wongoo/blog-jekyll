---
author: admin
comments: true
date: 2012-12-05 10:25:54+00:00
excerpt: Oracle7没有NLS_NCHAR_CHARACTERSET设置，只有NLS_CHARACTERSET设置。且Oracle7 varchar2只支持byte类型存储，没有char类型。Oracle7将字符转为NLS_CHARACTERSET编码的字符再存入varchar2字段，但特殊难字转换失败时就会变为问号。由于Oracle7支持CHR方法(将字符编号转为对应的字符)，所以将可以在执行sql之前将sql语法中的难字全部替换为CHR方法转换即可.
layout: post
slug: oracle-jdbc-special-char-encode
title: Oracle数据库JDBC难字编码问题
wordpress_id: 372
categories:
- Experience
tags:
- characterset
- encode
- oracle7
---

客户的数据库是Oracle7，Oracle7的JDBC只有基于java1.0开发的，所以改用支持java1.4的Oracle9的JDBC(ojdbc14.jar).
遇到一个难字insert到Oracle7之后变为问号。查询相关资料了解到，varchar2会存储字符时会以NLS_CHARACTERSET设置的编码进行存储。在NLS_CHARACTERSET=ZHT16BIG5,Oracle10g针对一些特殊自造字，能够由UTF8转换为BIG5编码，但oracle7却不行，转换失败时就会变为问号，应该是版本太早转换字符的映射表范围有限。 那么有没有可能扩展其编码转换映射表呢？（欢迎讨论）

为了解决此问题，变换一种方式：由于Oracle7支持CHR方法(将字符编号转为对应的字符)，所以将可以在执行sql之前将sql语法中的难字全部替换为CHR方法转换即可.


    String sql = "insert into TC VALUES('hello 銹 how are you ')";
    sql = sql.replace("銹", "'|| CHR('63959') || '");
    System.out.println(sql); 
    // insert into TC VALUES('hello '|| CHR('63959') || ' how are you ');
    


已发现不可正常转码的难字：

    NAME RAWTOHEX(NAME)
    ------- ------------
    碁 F9D6
    銹 F9D7
    裏 F9D8
    墻 F9D9
    恒 F9DA
    粧 F9DB
    嫺 F9DC 




* * *



1. What is national character set / NLS_NCHAR_CHARACTERSET?,[http://arjudba.blogspot.tw/2009/02/what-is-national-character-set.html](http://arjudba.blogspot.tw/2009/02/what-is-national-character-set.html)

    The national character set is the character set which is defined in oracle database in addition to normal character set.
    The normal character set is defined by the parameter NLS_CHARACTERSET and the national character set is defined by the parameter NLS_NCHAR_CHARACTERSET.
    The national character set is used for data stored in NCHAR, NVARCHAR2 and NCLOB columns while the normal character set is used for data stored in CHAR, VARCHAR2, CLOB columns.
    NLS_NCHAR_CHARACTERSET is defined when the database is created and specified with the CREATE DATABASE command.
    The default value of NLS_NCHAR_CHARACTERSET is AL16UTF16.
    From Oracle 9i onwards the NLS_NCHAR_CHARACTERSET can have only 2 values, either UTF8 or AL16UTF16 and both are unicode character sets.
    National character set are always defined in CHAR length semantics and you cannot define them in BYTE. That means if you defines NCHAR(5) then 5 maximum characters can be stored regardless of how many bytes they can hold.
    Many one thinks that they need to use the NLS_NCHAR_CHARACTERSET to have UNICODE support in oracle but this is not true.
    One can always use UNICODE in either two ways. Storing data into NCHAR, NVARCHAR2 or NCLOB columns or you can perfectly use "normal" CHAR and VARCHAR2 columns for storing unicode in a database who has a AL32UTF8 / UTF8 NLS_CHARACTERSET.


2. Oracle Setting:

    select name,value$ from sys.props$;
    select property_name,property_value,description from database_properties;
    select parameter,value from nls_database_parameters;


3. About Oracle JDBC encode , [http://stackoverflow.com/questions/8014700/how-do-i-set-character-encoding-for-oracle-10g-with-jdbc](http://stackoverflow.com/questions/8014700/how-do-i-set-character-encoding-for-oracle-10g-with-jdbc)

    [Chris:]The data transferred by the thin Oracle JDBC driver is always sent as UTF-16 (java's internal representation).
    The database server will translate that into whatever national character set it has been configured
    to use (so if the database was set up to be UTF-8, this conversion will happen automatically).
    Note that the character set is set at the Database level, not at the schema or connection level.
    

4. VARCHAR2 only support byte storage in Oracle7,[http://docs.oracle.com/cd/A57673_01/DOC/server/doc/SQL73/ch2.htm#datatypesumm](http://docs.oracle.com/cd/A57673_01/DOC/server/doc/SQL73/ch2.htm#datatypesumm)

    VARCHAR2(size) Variable length character string having maximum length size bytes. Maximum size is 2000, and minimum is 1. You must specify size for a VARCHAR2


5. BIG5 Encode Scope , [http://en.wikipedia.org/wiki/Big5](http://en.wikipedia.org/wiki/BIG5)

    In the original Big5, the encoding is compartmentalized into different zones:
    0x8140 to 0xA0FE Reserved for user-defined characters 造字
    0xA140 to 0xA3BF "Graphical characters" 圖形碼
    0xA3C0 to 0xA3FE Reserved, not for user-defined characters
    0xA440 to 0xC67E Frequently used characters 常用字
    0xC6A1 to 0xC8FE Reserved for user-defined characters
    0xC940 to 0xF9D5 Less frequently used characters 次常用字
    0xF9D6 to 0xFEFE Reserved for user-defined characters


6. BIG5 database Map:
[http://www.khngai.com/chinese/charmap/tblbig.php?page=5](http://www.khngai.com/chinese/charmap/tblbig.php?page=5)
[http://smontagu.damowmow.com/genEncodingTest.cgi?family=windows&codepage=950](http://smontagu.damowmow.com/genEncodingTest.cgi?family=windows&codepage=950)

7. Oracle CHR function , [http://docs.oracle.com/cd/B19306_01/server.102/b14200/functions019.htm](http://docs.oracle.com/cd/B19306_01/server.102/b14200/functions019.htm)

8. Function for converting HEX to Decimal in Oracle(for all versions):

    CREATE OR REPLACE FUNCTION hex2dec (hexnum IN CHAR) RETURN NUMBER IS
        i NUMBER;
        digits NUMBER;
        result NUMBER := 0;
        current_digit CHAR(1);
        current_digit_dec NUMBER;
    BEGIN
        digits := LENGTH(hexnum);
        FOR i IN 1..digits LOOP
            current_digit := SUBSTR(hexnum, i, 1);
            IF current_digit IN ('A','B','C','D','E','F') THEN
                current_digit_dec := ASCII(current_digit) - ASCII('A') + 10;
            ELSE
                current_digit_dec := ASCII(current_digit) - ASCII('0');
            END IF;
            result := (result * 16) + current_digit_dec;
        END LOOP;
        RETURN result;
    END hex2dec;

