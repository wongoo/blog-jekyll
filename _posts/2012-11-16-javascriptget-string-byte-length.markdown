---
author: admin
comments: true
date: 2012-11-16 10:13:34+00:00
excerpt: Javascript:Get string byte length
layout: post
slug: javascriptget-string-byte-length
title: Javascript:Get string byte length
wordpress_id: 351
categories:
- Experience
tags:
- charCodeAt
- encode
- encodeURI
- Javascript
- unicode
---


    
    /*
     * encodeURI replaces each instance of certain characters by one, two, three, or four escape sequences
     * representing the UTF-8 encoding of the character.
     * Each one in the escape sequences is represented in the format of ‘%NN’, where ‘N’ is one Hex number.
     *
     * @return the UTF-8 byte length of a string.
    */
    function utf8ByteLength(str) {
    	if (!str) return 0;
    	var escapedStr = encodeURI(str);
    	var match = escapedStr.match(/%/g);
    	return match ? (escapedStr.length - match.length *2) : escapedStr.length;
    }
    /*
     * The method charCodeAt returns the numeric Unicode value of the character at the given index.
     *
     * @return the unicode byte length of a string.
     */
    function unicodeByteLength(str) {
    	if (!str) return 0;
    	var ch;
    	var count = 0;
    	for ( var i = 0; i < str.length; i++) {
    		ch = str.charCodeAt(i);
    		do {
    			count++;
    			ch = ch >> 8; // shift value down by 1 byte
    		} while (ch);
    	}
    	return count;
    }
    /*
     * The DBSC encodes , like BIG5,GBK , use two bytes to represent a none-ASCII character, which is represented by multiple bytes in Unicode.
     *
     * @return the double-byte character set (DBCS) byte length of a string.
     */
    function dbcsByteLength(str) {
    	if (!str) return 0;
    	var count = 0;
    	for ( var i = 0; i < str.length; i++) {
    		count++;
    		if(str.charCodeAt(i) >> 8) count++;
    	}
    	return count;
    }
    


**Reference:**
1. Reading bytes from a JavaScript string , [http://stackoverflow.com/questions/1240408/reading-bytes-from-a-javascript-string](http://stackoverflow.com/questions/1240408/reading-bytes-from-a-javascript-string)
2. Unicode, [http://en.wikipedia.org/wiki/UNICODE](http://en.wikipedia.org/wiki/UNICODE)
3. UTF-8, [http://en.wikipedia.org/wiki/Utf8](http://en.wikipedia.org/wiki/Utf8)
4. BIG5, [http://en.wikipedia.org/wiki/BIG5](http://en.wikipedia.org/wiki/BIG5)
5. GBK, [http://en.wikipedia.org/wiki/GBK](http://en.wikipedia.org/wiki/GBK)
6. Javascript charCodeAt, [https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/String/charCodeAt](https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/String/charCodeAt)
7. Javascript encodeURI, [https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/encodeURI](https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/encodeURI)
