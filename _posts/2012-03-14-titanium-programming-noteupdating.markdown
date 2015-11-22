---
author: admin
comments: true
date: 2012-03-14 08:49:22+00:00
layout: post
slug: titanium-programming-noteupdating
title: '[updating] Titanium Programming Note'
wordpress_id: 235
categories:
- Experience
tags:
- ipad
- Mobile
- Titanium
---

### Page Architecture


****Base Window widget：**TabGroup、Window
you can ：
**



	
  1. add Tab into TabGroup

	
  2. add Window into Tab

	
  3. add View into  Window

	
  4. add other widget into Window ( like table,lable ...)


Bast Practice:

	
  1.     Add View into Window

	
  2.     Add other widget into View





* * *





### Scree size


Titanium.Platform.displayCaps.platformWidth,
Titanium.Platform.displayCaps.platformHeight,





* * *





### File upload


[http://www.smokycogs.com/blog/titanium-tutorial-how-to-upload-a-file-to-a-server/](http://www.smokycogs.com/blog/titanium-tutorial-how-to-upload-a-file-to-a-server/)

**List Files:**

    var resourcesDir = Titanium.Filesystem.resourcesDirectory();
            var dir = Titanium.Filesystem.getFile(resourcesDir);
            var dir_files = dir.getDirectoryListing();
            for (var i=1;i<dir_files.length;i++){
               Ti.API.log(dir_files[i]);
            }




* * *





### repeat background image


[http://developer.appcelerator.com/question/70131/repeat-background-image](http://developer.appcelerator.com/question/70131/repeat-background-image)
