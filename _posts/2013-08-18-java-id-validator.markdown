---
author: admin
comments: true
date: 2013-08-18 09:15:59+00:00
excerpt: Java身份证号码验证器
layout: post
slug: java-id-validator
title: Java身份证号码验证器
wordpress_id: 475
categories:
- Experience
tags:
- java
- Validator
---



    
     import java.text.ParseException;
     
     /**
      * 身份证号码验证   

      * 1、号码的结构   

      * 公民身份号码是特征组合码，由十七位数字本体码和一位校验码组成。排列顺序从左至右依次为：六位数字地址码，   

      * 八位数字出生日期码，三位数字顺序码和一位数字校验码。   

      * 2、地址码(前六位数）   

      * 表示编码对象常住户口所在县(市、旗、区)的行政区划代码，按GB/T2260的规定执行。   

      * 3、出生日期码（第七位至十四位）   

      * 表示编码对象出生的年、月、日，按GB/T7408的规定执行，年、月、日代码之间不用分隔符。   

      * 4、顺序码（第十五位至十七位）   

      * 表示在同一地址码所标识的区域范围内，对同年、同月、同日出生的人编定的顺序号，   

      * 顺序码的奇数分配给男性，偶数分配给女性。   

      * 5、校验码（第十八位数）   

      * （1）十七位数字本体码加权求和公式 S = Sum(Ai * Wi), i = 0, , 16 ，先对前17位数字的权求和   

      * Ai:表示第i位置上的身份证号码数字值 Wi:表示第i位置上的加权因子 Wi: 7 9 10 5 8 4 2 1 6 3 7 9 10 5 8 4   

      * 2 （2）计算模 Y = mod(S, 11) （3）通过模得到对应的校验码 Y: 0 1 2 3 4 5 6 7 8 9 10 校验码: 1 0   

      * X 9 8 7 6 5 4 3 2   

      * 
      * @author Geln Yang
      * 
      */
     public class IdentityCardUtil {
     
     	private static final String IDENTITY_NO_REGEX = "([\\d]{15})|([\\d]{17}(\\d|[xX]))";
     	private static final String DATE_REGEX = "^((\\d{2}(([02468][048])|([13579][26]))((((0?[13578])|(1[02]))((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))((0?[1-9])|([1-2][0-9])|(30)))|(0?2((0?[1-9])|([1-2][0-9])))))|(\\d{2}(([02468][1235679])|([13579][01345789]))((((0?[13578])|(1[02]))((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))((0?[1-9])|([1-2][0-9])|(30)))|(0?2((0?[1-9])|(1[0-9])|(2[0-8]))))))(\\s(((0?[0-9])|([1-2][0-3]))\\:([0-5]?[0-9])((\\s)|(\\:([0-5]?[0-9])))))?$";
     	private static char[] VALIDATE_CODE_ARR = { '1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2' };
     	private static int[] POWSER_ARR = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
     	private static int[] AREA_CODE_ARR = new int[100];
     
     	static {
     		for (int i = 0; i < 100; i++) {
     			AREA_CODE_ARR[i] = 0;
     		}
     		AREA_CODE_ARR[11] = 1;// 北京
     		AREA_CODE_ARR[12] = 1;// 天津
     		AREA_CODE_ARR[13] = 1;// 河北
     		AREA_CODE_ARR[14] = 1;// 山西
     		AREA_CODE_ARR[15] = 1;// 内蒙古
     		AREA_CODE_ARR[21] = 1;// 辽宁
     		AREA_CODE_ARR[22] = 1;// 吉林
     		AREA_CODE_ARR[23] = 1;// 黑龙江
     		AREA_CODE_ARR[31] = 1;// 上海
     		AREA_CODE_ARR[32] = 1;// 江苏
     		AREA_CODE_ARR[33] = 1;// 浙江
     		AREA_CODE_ARR[34] = 1;// 安徽
     		AREA_CODE_ARR[35] = 1;// 福建
     		AREA_CODE_ARR[36] = 1;// 江西
     		AREA_CODE_ARR[37] = 1;// 山东
     		AREA_CODE_ARR[41] = 1;// 河南
     		AREA_CODE_ARR[42] = 1;// 湖北
     		AREA_CODE_ARR[43] = 1;// 湖南
     		AREA_CODE_ARR[44] = 1;// 广东
     		AREA_CODE_ARR[45] = 1;// 广西
     		AREA_CODE_ARR[46] = 1;// 海南
     		AREA_CODE_ARR[50] = 1;// 重庆
     		AREA_CODE_ARR[51] = 1;// 四川
     		AREA_CODE_ARR[52] = 1;// 贵州
     		AREA_CODE_ARR[53] = 1;// 云南
     		AREA_CODE_ARR[54] = 1;// 西藏
     		AREA_CODE_ARR[61] = 1;// 陕西
     		AREA_CODE_ARR[62] = 1;// 甘肃
     		AREA_CODE_ARR[63] = 1;// 青海
     		AREA_CODE_ARR[64] = 1;// 宁夏
     		AREA_CODE_ARR[65] = 1;// 新疆
     		AREA_CODE_ARR[71] = 1;// 台湾
     		AREA_CODE_ARR[81] = 1;// 香港
     		AREA_CODE_ARR[82] = 1;// 澳门
     		AREA_CODE_ARR[91] = 1;// 国外
     	}
     
     	/**
     	 * 功能：身份证的有效验证
     	 * 
     	 * @param identityNo
     	 * @return true/false
     	 */
     	public static boolean validate(String identityNo) {
     		if (identityNo == null) {
     			return false;
     		}
     		if (!identityNo.matches(IDENTITY_NO_REGEX)) {
     			return false;
     		}
     		if (AREA_CODE_ARR[Integer.parseInt(identityNo.substring(0, 2))] == 0) {
     			return false;
     		}
     		String date = identityNo.length() == 15 ? "19" + identityNo.substring(6, 12) : identityNo.substring(6, 14);
     		if (!date.matches(DATE_REGEX)) {
     			return false;
     		}
     		if (identityNo.length() == 15) {
     			return true;
     		}
     		int checksum = 0;
     		for (int i = 0; i < 17; i++) {
     			checksum = checksum + (identityNo.charAt(i) - 48) * POWSER_ARR[i];
     		}
     		char verifyCode = VALIDATE_CODE_ARR[checksum % 11];
     		if (identityNo.charAt(17) != verifyCode) {
     			return verifyCode == 'X' ? identityNo.charAt(17) == 'x' : false;
     		}
     		return true;
     	}
     
     	public static char calculateCheckNum(char[] out) {
     		int checksum = 0;
     		for (int i = 0; i < 17; i++) {
     			checksum = checksum + (out[i] - 48) * POWSER_ARR[i];
     		}
     		return VALIDATE_CODE_ARR[checksum % 11];
     	}
     
     	public static void main(String[] args) throws ParseException {
     		testValid("210102820826411");
     		testValid("21010219820826411");
     		testValid("210102198208264114");
     		testValid("500113198606245216");
     	}
     
     	private static void testValid(String c) {
     		System.out.println(c + ":" + validate(c));
     	}
     
     }
    

