---
author: admin
comments: true
date: 2014-01-24 01:53:29+00:00
excerpt: Javascript动态加载国际化内容
layout: post
slug: javascript-dynamic-load-i18n
title: Javascript动态加载国际化内容
wordpress_id: 586
categories:
- Experience
tags:
- i18n
- international
- Javascript
---

定义一个javascirpt方法，传入需要加载国际化消息key键数组，获得这些消息加载到页面中。

    
    /*
     load i18n resource through javascript
     */
    function loadi18n(keys) {
    	if (keys == null || keys.length == 0) {
    		return;
    	}
    	if(!window.i18n){var i18n = window.i18n = {}}
    	jQuery(document).ready(function(){
    		var method = keys.length > 20 ? "POST" : "GET";
    		jQuery.ajax({
    			url : context_path + "/js/resource",
    			data:{"key[]":keys},
    			type:method,
    			cache:true,
    			dataType:"script",
    			success:function(data){}
    		})}
    	)
    }


以下是一个Spring Controller根据传入key键返回国际化消息：

    
    @RequestMapping(value = "/js/resource")
    public void getMessages(@RequestParam(value = "key[]") String[] keys, HttpServletRequest request, HttpServletResponse response, Locale locale) {
    	ResourceBundle resourceBundle = ResourceBundle.getBundle("message/messages", locale, JsI18nController.class.getClassLoader());
    	response.setCharacterEncoding("utf-8");
    	response.setContentType("application/javascript");
    	StringBuilder contents = new StringBuilder();
    	if (keys != null && keys.length > 0) {
    		for (String key : keys) {
    			String value = resourceBundle.getString(key);
    			contents.append("i18n['" + key + "']=\"" + value + "\";");
    		}
    		try {
    			response.getWriter().write(contents.toString());
    		} catch (IOException e) {
    			logger.error(e.getMessage(), e);
    		}
    	}	
    }


以下是javascript动态调用获取国际化例子：

    
    loadi18n([ "sure.delete", "success.delete" ]);
    loadi18n([ "failure.delete" ]); //根据需要可多次反复调用
    
    function delUser(userId) {
    	if (confirm(i18n['sure.delete'])) {
    		// to delete the user
    	}
    }
    

