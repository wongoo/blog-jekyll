---
layout: default
title: Build Website with Github Pages
---
#{{ page.title }}

##1. Github Pages Documents：
[GitHub Pages Basics](https://help.github.com/categories/github-pages-basics/)

[How GitHub Pages sites use custom domains](https://help.github.com/articles/about-custom-domains-for-github-pages-sites/)
,try to understand how the page is redirected.
		
##2. Create the MAIN site repository:
1. create repository: username.github.io
2. create index page index.html
3. create file CNAME:

		example.com
4. push under branch: master
5. get ip address:

		ping username.github.io
6. configure DNS A record for host "@" and "www":

		主机名	类型	对应值			TTL
		@		A		103.245.222.133	900	-	
		www		A		103.245.222.133	900	-

7. then you can visit: http://example.com

@see [adding a cname file to your repository](https://help.github.com/articles/adding-a-cname-file-to-your-repository/)

##3. Create the BLOG site repository:
1. create repository: blog.example.com
2. create index page index.html
3. create file CNAME:example.com
4. push under branch: gh-pages
5. then you can visit: http://example.com/blog

{{ page.date | date_to_string }}
