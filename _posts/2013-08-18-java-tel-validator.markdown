---
author: admin
comments: true
date: 2013-08-18 09:36:32+00:00
excerpt: Java电话号码验证器
layout: post
slug: java-tel-validator
title: Java电话号码验证器
wordpress_id: 480
categories:
- Experience
tags:
- java
- Validator
---



    
    import java.util.regex.Matcher;
    import java.util.regex.Pattern;
    
    /**
     * 根据实际开发于2009年9月7日最新统计：  

     * 中国电信发布中国3G号码段:中国联通185,186;中国移动188,187;中国电信189,180共6个号段。  

     * 3G业务专属的180-189号段已基本分配给各运营商使用, 其中180、189分配给中国电信,187、188归中国移动使用,185、186属于新联通。  

     * 中国移动拥有号码段：139、138、137、136、135、134、159、158、157（3G）、152、151、150、188（3G）、187（3G） ;14个号段  

     * 中国联通拥有号码段：130、131、132、155、156（3G）、186（3G）、185（3G）;6个号段  

     * 中国电信拥有号码段：133、153、189（3G）、180（3G）;4个号码段  

     * 移动:  

     * 2G号段(GSM网络)有139,138,137,136,135,134(0-8),159,158,152,151,150  

     * 3G号段(TD-SCDMA网络)有157,188,187  

     * 147是移动TD上网卡专用号段.  

     * 联通:  

     * 　2G号段(GSM网络)有130,131,132,155,156  

     * 3G号段(WCDMA网络)有186,185  

     * 电信:  

     * 2G号段(CDMA网络)有133,153  

     * 3G号段(CDMA网络)有189,180  

     *   

     *   

     * 座机号码:  

     * 三位区号的部分  

     * 010， 021-029，852（香港）  

     * 因为采用三位区号的地方都是8位电话号码，因此可以写成  

     * 再看4位区号的城市,这里简单判断了不可能存在0111或者0222的区号，以及电话号码是7位或者8位。   

     * 
     * @author Geln Yang
     * 
     */
    public class PhoneNoUtil {
    	private static String MobileMatchStr = "((13[0-9]{1})|(15[^4,\\D])|(18[0,5-9]{1}))\\d{8}";
    	private static String DIRECT_DISTRICT_TEL_REGEX = "(010|021|022|023|024|025|026|027|028|029|852)\\-?\\d{8}";
    	private static Pattern DIRECT_DISTRICT_TEL_PATTERN = Pattern.compile(DIRECT_DISTRICT_TEL_REGEX);
    	private static String PROVINCE_TEL_REGEX = "(0[3-9][1-9]{2})\\-?\\d{6,8}((\\-\\d{1,6})|(\\(\\d{1,6}\\)))?";
    	private static Pattern PROVINCE_TEL_PATTERN = Pattern.compile(PROVINCE_TEL_REGEX);
    	private static String TEL_REGEX = "((" + DIRECT_DISTRICT_TEL_REGEX + ")|(" + PROVINCE_TEL_REGEX + "))";
    
    	public static boolean isMobile(String no) {
    		return no != null ? no.matches(MobileMatchStr) : false;
    	}
    
    	public static boolean isTel(String no) {
    		return no != null ? no.matches(TEL_REGEX) : false;
    	}
    
    	public static boolean isDirectDistrictTel(String no) {
    		return no != null ? no.matches(DIRECT_DISTRICT_TEL_REGEX) : false;
    	}
    
    	public static String getDirectDistrictTelAreaCode(String no) {
    		if (no != null) {
    			Matcher matcher = DIRECT_DISTRICT_TEL_PATTERN.matcher(no);
    			if (matcher.matches()) {
    				return matcher.group(1);
    			}
    		}
    		return null;
    	}
    
    	public static boolean isProvinceTel(String no) {
    		return no != null ? no.matches(PROVINCE_TEL_REGEX) : false;
    	}
    
    	public static String getProvinceTelAreaCode(String no) {
    		if (no != null) {
    			Matcher matcher = PROVINCE_TEL_PATTERN.matcher(no);
    			if (matcher.matches()) {
    				return matcher.group(1);
    			}
    		}
    		return null;
    	}
    
    	public static boolean validate(String no) {
    		return no != null ? no.matches(MobileMatchStr) || no.matches(TEL_REGEX) : false;
    	}
    
    	public static void main(String[] args) {
    		testValid("13779946436");
    		testValid("05923928777(833)");
    		testValid("15923928777(833)");
    		testValid("0592-23928777-833");
    		testValid("010-79311332");
    		testValid("10-79311332");
    		System.out.println(getProvinceTelAreaCode("0592-23928777-833"));
    		System.out.println(getDirectDistrictTelAreaCode("010-79311332"));
    	}
    
    	private static void testValid(String no) {
    		System.out.println(no + ":" + validate(no));
    	}
    }
    

