---
author: admin
comments: true
date: 2013-12-16 15:22:56+00:00
excerpt: maven介绍，POM介绍，传递依赖，管理依赖，引用依赖，repository管理，mirror镜像管理
layout: default
slug: using-maven-to-manage-project
title: 使用Maven管理项目开发
wordpress_id: 542
categories:
- Knowledge
tags:
- apache
- dependency
- maven
---

## **目录**


1. 关于Maven
2. 关于POM
3. 关于Repository
4. 关于Settings
5. 关于Repository Mirror
6. 标准文件结构：
7. 依赖机制
8. 引用依赖(Importing Dependencies)
9. 系统依赖(System Dependencies)
10. FAQs

### **1.关于Maven**


Maven开发者的愿景：
We wanted a standard way to build the projects, a clear definition of what the project consisted of, an easy way to publish project information and a way to share JARs across several projects.

The result is a tool that can now be used for building and managing any Java-based project. We hope that we have created something that will make the day-to-day work of Java developers easier and generally help with the comprehension of any Java-based project.

Maven的目标：

* Making the build process easy – 让创建过程变动容易
* Providing a uniform build system – 提供一个统一的创建部署系统
* Providing quality project information – 提供有质量的项目信息支持
* Providing guidelines for best practices development – 提供最佳开发实际指导
* Allowing transparent migration to new features – 允许无缝整合新的功能特性

参考：

1. [What is maven](http://maven.apache.org/what-is-maven.html)
2. [Maven Documents](http://maven.apache.org/guides/index.html)
3. [Maven Reference](http://books.sonatype.com/mvnref-book/reference/)

### **2.关于POM**


POM全称 Project Object Model(项目对象模型)，它是Maven工作的基础。

POM中包含以下信息：

* Dependencies – libraries依赖关系
* developers and contributors – 开发者和共享者信息
* plugin lists (including reports) – 插件列表
* plugin executions with matching ids – id关联的插件执行
* plugin configuration – 插件配置
* resources – 其他资源信息

POM中并不需要全部配置这些信息，POM配置通过继承方式继承parent POM（父POM）或super POM（顶级POM）的配置（另外repository相关配置信息还继承于settings.xml），这样极大简化配置要求。

最简单的POM只需配置modelVersion、groupdId、artifactId、version即可，其他信息则继承于super POM和seetings.xml,如：

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>com.mycompany.app</groupId>
	  <artifactId>my-module</artifactId>
	  <version>1</version>
	</project>


如果是继承于parent，需声明parent的groupId、artifactId、version、relativePath（Parent POM位置）；如果groupId和version同parent一致，则无需再声明; 如果该模块要打包为jar，则可声明packaging为jar；如：

	<project>
	  <parent>
	    <groupId>com.mycompany.app</groupId>
	    <artifactId>my-app</artifactId>
	    <version>1</version>
	    <relativePath>..</relativePath>
	  </parent>
	  <modelVersion>4.0.0</modelVersion>
	  <artifactId>my-module</artifactId>
	<packaging>jar</packaging>
	</project>



Parent模块的POM需声明packaging方式为POM，并指定子模块modules，如下：

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>com.mycompany.app</groupId>
	  <artifactId>my-app</artifactId>
	  <version>1</version>
	  <packaging>pom</packaging>
	
	  <modules>
	    <module>my-module</module>
	  </modules>
	</project>


声明依赖关系的方式如下（下例中还说明如何声明一个变量）：

	<project>
	  ...
	  <properties>
	    <mavenVersion>2.1</mavenVersion>
	  </properties>
	  <dependencies>
	    <dependency>
	      <groupId>junit</groupId>
	      <artifactId>junit</artifactId>
	      <version>4.11</version>
	    </dependency>
	    <dependency>
	      <groupId>org.apache.maven</groupId>
	      <artifactId>maven-artifact</artifactId>
	      <version>${mavenVersion}</version>
	    </dependency>
	    <dependency>
	      <groupId>org.apache.maven</groupId>
	      <artifactId>maven-project</artifactId>
	      <version>${mavenVersion}</version>
	    </dependency>
	  </dependencies>
	  ...
	</project>


关于super POM:

All Maven project POMs extend the Super POM, which defines a set of defaults shared by all projects. This Super POM is a part of the Maven installation. Depending on the Maven version it can be found in the maven-x.y.z-uber.jar or maven-model-builder-xy.z.jar file in ${M2_HOME}/lib. If you look in this JAR file, you will find a file named pom-4.0.0.xml under the org.apache.maven.model package. It is also published on the Maven reference site
This setting can be overridden by a custom settings.xml file.


参考：<a href="http://maven.apache.org/guides/introduction/introduction-to-the-pom.html">introduction to the pom</a>


<h3><b>3.关于Repository</b></h3>

Repository用于存放artifacts以及各种依赖相关文件。
Repository分两种：local、remote；local repository指本地文件缓存，其结构跟remote repository一样，便于同步比较更新； remote repository分为internal和internet两种，建立内网internal repository是为了下载方便（更快速、更好管理）；internal repository有时作为repository manager，可以proxy多个外网repository，而只需一个配置一个internal repository地址即可。

Repository软件参考：Nexus，Artifacotry

参考：<a href="http://maven.apache.org/guides/introduction/introduction-to-repositories.html">introduction to repositories</a>



<h3><b>4.关于Settings</b></h3>

默認settings配置文件是：~/.m2/settings.xml，首先需要配置本地repository存放位置：

	<settings>
	  ...
		<localRepository>f:/maven_repository</localRepository>
		...
	</settings>


配置repository，先聲明一個profile，聲明其id和下載屬性，然後配置repositories和pluginRepositories； 最後在activeProfile中聲明那一個profile是生效的：

	<settings>
	<localRepository>f:/maven_repository</localRepository>
	<profiles>
	<profile>
	    <id>artifactory</id>
	    <properties>
	        <downloadSources>true</downloadSources>
	        <downloadJavadocs>true</downloadJavadocs>           
	    </properties>
		<repositories>
			  <repository>
					<id>central</id>
					<url>http://192.168.1.10:9999/nexus/content/groups/public</url>
					<snapshots>
						  <enabled>true</enabled>
					</snapshots>
			  </repository>
		</repositories>
		<pluginRepositories>
			<pluginRepository>
					<id>central</id>
					<url>http://192.168.1.10:9999/nexus/content/groups/public</url>
					<snapshots>
						  <enabled>false</enabled>
					</snapshots>
			  </pluginRepository>
		</pluginRepositories>
	</profile>
	</profiles>
	
	<activeProfiles>
	  <activeProfile>artifactory</activeProfile>
	</activeProfiles>
	
	</settings>


<strong>Repository訪問先後順序：</strong>
<ol>
<li>优先访问settings.xml中的配置的repository，然后再访问pom.xml中配置的repository；</li>
<li>如果setting.xml和pom.xml中的repository的id重复，则直接使用setting.xml中的配置；</li>
<li>如果setting.xml和pom.xml中都未配置repository，则使用默认的repository：http://repo.maven.apache.org/maven2</li>
</ol>
參考：
<a href="http://maven.apache.org/ref/2.2.1/maven-settings/settings.html">Settings配置説明</a>


<h3><b>5.关于Repository Mirror</b></h3>
Repository Mirror (鏡像)用於轉發一個或多個對repository的請求到指定的Mirror (鏡像)服務器下載。以下幾種情景需要用到Repository Mirror (鏡像)：
鏡像服務器離得跟進，訪問速度更快；
用本地的Repository服務替換特定的Repository服務，以便更好的管理；
運行Repository Manager作本地緩存；

鏡像配置方式：

	<settings>
	  ...
	  <mirrors>
	    <mirror>
	      <id>UK</id>
	      <name>UK Central</name>
	      <url>http://uk.maven.org/maven2</url>
	      <mirrorOf>central</mirrorOf>
	    </mirror>
	  </mirrors>
	  ...
	</settings>

如上，將對central （repository ID）repository的訪問全部轉向到http://uk.maven.org/maven2鏡像；

<strong>mirrorOf有多种配置方式：</strong>
<ol>
<li>	*：表示所有的repository</li>
<li>	xxx，yyy：表示repository的id名稱，如果要轉發多個repository，則多個id值用逗號區隔；</li>
<li>	*,!repo1 ：表示所有repository但除開repo1</li>
<li>	external:* ：表示所有repository，但除開本地以及文件訪問路徑的repository（非http）</li>
</ol>
可以強制將所有repository請求都轉發到内網repository服務器上，可以提高訪問速度的同時也便於管理：

	<settings>
	  ...
	  <mirrors>
	    <mirror>
	      <id>internal-repository</id>
	      <name>Maven Repository Manager running on repo.mycompany.com</name>
	      <url>http://repo.mycompany.com/proxy</url>
	      <mirrorOf>*</mirrorOf>
	    </mirror>
	  </mirrors>
	  ...
	</settings>


參考：<a href="http://maven.apache.org/guides/mini/guide-mirror-settings.html">maven mirros settings</a>


<h3><b>6.标准文件结构</b></h3>
Maven项目默认的标准文件结构如下：

	src/main/java	    Application/Library sources
	src/main/resources	Application/Library resources
	src/main/filters	Resource filter files
	src/main/assembly	Assembly descriptors
	src/main/config     Configuration files
	src/main/scripts	Application/Library scripts
	src/main/webapp	    application sources
	src/test/java	    Test sources
	src/test/resources	Test resources
	src/test/filters	Test resource filter files
	src/site	        Site
	LICENSE.txt         Project's license
	NOTICE.txt	        Notices and attributions required by libraries that the project depends on
	README.txt	        Project's readme

顶层子目录有src和target，target是编译部署相关的输出目录。
参考：<a href="http://maven.apache.org/guides/introduction/introduction-to-the-standard-directory-layout.html">maven standard directory layout</a>

<h3><b>7.依赖机制</b></h3>
<strong>Transitive dependencies传递依赖：</strong>
不用 显式的声明所有依赖，maven会自动将所有依赖包含进来；不管依赖数量或依赖层级有多少，但是如果发现循环依赖，则会出错。
因为传递依赖的特性，可能会导致依赖的library数量不断增大；Maven提供了一些限制加入依赖的特性：
<ol>
<li>Dependency mediation(依赖仲裁) – 当遇到多个版本的library的时候决定使用哪一个版本；Maven2.0开始支持“nearest definition”最短定义机制，它会选择使用依赖树中位置最靠近的依赖版本；但是也可以明确的再POM中声明需要使用那一个版本（对主要的依赖建议明确声明，如使用Spring MVC会自动依赖引入Spring core,但是明确声明Spring Core的版本是比较好的选择，比如Spring Security所需的Spring Core版本可能与Spring MVC不一样）。如果在依赖树中同一层级发现两个版本，从Maven2.0.9开始，会使用第一个声明的版本；"nearest definition"（最短定义）说明：如声明了依赖A,B,C, 其中有依赖关系 A -> B -> C -> D 2.0 和A -> E -> D 1.0, 这个时候就会选择D 1.0 ，因为A到D的路径通过E最短. 但是引入D1.0会导致B,C无法正常依赖，此时可以明确的添加依赖声明依赖D2.0.</li>
<li>Dependency Management（依赖管理）- 直接声明所需library的版本。在上例中D间接被A引用，尽管已被引用，但可以再配置中明确的声明所需版本。</li>
<li>Dependency scope（依赖范围）- 允许你在当前build阶段只包含适合的依赖。</li>
<li>Excluded dependencies (非包含依赖)- 如果X依赖Y, Y依赖Z, 但可以通过声明“exclusion”标签明确去除对Z的依赖。</li>
<li>Optional dependencies （选择依赖）- Y依赖Z，Y可以把Z通过optional标签表明为选择依赖。X依赖Y，但X不会传递依赖Z. X只有明确声明依赖Z才会生效。选择依赖可以理解为“默认的非包含依赖”。</li>
</ol>

<strong>Dependency scope（依赖范围）</strong>
依赖范围用于限制依赖传递，同时影响各种build任务的classpath引用范围；
有6中依赖范围:
<ol>
<li>compile（编译范围）- 默认范围。编译范围的依赖再所有项目的classpath都是可以引用到的。且这些依赖可以传递到所有子项目中。</li>
<li>provided（供给范围）- 很像compile编译范围，但它表明在运行时由JDK或者container提供这些依赖。例如，创建企业级java web应用，你可能会设置servlet和其他一些JavaEE的范围为provided，因为这些library会由container提供。供给范围只会在当前项目的编译和测试的classpath可以引用。不具传递性！</li>
<li>runtime（运行时范围）- 此范围表明在编译的时候是不需要的，只有在运行时才需要。在运行时和测试的classpath才可以引用到。</li>
<li>test(测试范围)- 此范围表明在一般情况下不会使用到，只有在测试编译或测试运行的时候才会引用到。</li>
<li>system（系统范围）- 此范围类似provided供给范围，但除了一点，那就是你必须明确的在项目中提供这个jar档，maven认为它任何时候都是引用到的，不会从任何repository查看。</li>
<li>import (2.0.9或之后的版本)（引用范围）- 此范围仅被用于packaging 为pom类型的<dependencyManagement>节点下的依赖声明，表明这个特殊的POM的依赖会被 <dependencyManagement> 节点下的声明所覆盖。既然是被覆盖了，这些import范围的依赖并不会实际限制传递依赖。</li>
</ol>
每一个范围（除了import引用范围）对传递依赖的影响不一样，如下表显示。

	依赖范围--> 传递依赖范围
	compile	--> compile(*)、runtime
	provided--> provided
	runtime	--> runtime
	test    --> test

注意：(*)表示这本应该是runtime范围，所以所有compile范围的依赖都需要明确的声明。但有一种情况，一个依赖的library继承了其他library的一个class，那么就会强制要求在compile的时候就需要引入。因为这个原因编译范围依赖任然保持为编译范围依赖，尽管它是可传递的。


<strong>dependencymanagement 和 dependencies的区别：</strong>
<ol>
<li>父项目的POM可以声明dependencies和dependencymanagement节点依赖；</li>
<li>Dependencies会被所有子项目的POM直接继承；</li>
<li>Dependencymanagement不会被子项目POM继承，但至项目可以在dependencies节点中声明所需的library，无需声明version版本；则此依赖会被父POM中Dependencymanagement定义的对应依赖声明覆盖。也就是说Dependencymanagement可以起到一个统一管理library版本信息的作用。同时因为其不会被子项目POM继承，这样就不会造成所有子项目都继承相同library，而其中很多library可能都是不需要的。</li>
</ol>

参考：

1. <a href="http://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html">Introduction to the Dependency Mechanism</a>
2. <a href="http://stackoverflow.com/questions/2619598/differences-between-dependencymanagement-and-dependencies-of-maven">differences between dependencymanagement and dependencies of maven</a>

<h3><b>8.引用依赖(Importing Dependencies)</b></h3>
Maven2.0.9开始加入此特性。之前的版本将不会解析import范围的声明。在考虑使用它之前请仔细权衡。如果一定要使用，建议使用enforce plugin强制使用Maven 2.0.9或以上版本。因为只能继承一个parent，可能不能满足一些大项目的要求，为了能够引用依赖多个项目，可以通过定义一个import范围的POM artifact。

Project A:

	<project>
	 <modelVersion>4.0.0</modelVersion>
	 <groupId>maven</groupId>
	 <artifactId>A</artifactId>
	 <packaging>pom</packaging>
	 <name>A</name>
	 <version>1.0</version>
	 <dependencyManagement>
	   <dependencies>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>a</artifactId>
	       <version>1.2</version>
	     </dependency>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>b</artifactId>
	       <version>1.0</version>
	       <scope>compile</scope>
	     </dependency>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>c</artifactId>
	       <version>1.0</version>
	       <scope>compile</scope>
	     </dependency>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>d</artifactId>
	       <version>1.2</version>
	     </dependency>
	   </dependencies>
	 </dependencyManagement>
	</project>


Project B:

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>maven</groupId>
	  <artifactId>B</artifactId>
	  <packaging>pom</packaging>
	  <name>B</name>
	  <version>1.0</version>
	  <dependencyManagement>
	    <dependencies>
	      <dependency>
	        <groupId>maven</groupId>
	        <artifactId>A</artifactId>
	        <version>1.0</version>
	        <type>pom</type>
	        <scope>import</scope>
	      </dependency>
	      <dependency>
	        <groupId>test</groupId>
	        <artifactId>d</artifactId>
	        <version>1.0</version>
	      </dependency>
	    </dependencies>
	  </dependencyManagement>
	  <dependencies>
	    <dependency>
	      <groupId>test</groupId>
	      <artifactId>a</artifactId>
	      <version>1.0</version>
	      <scope>runtime</scope>
	    </dependency>
	    <dependency>
	      <groupId>test</groupId>
	      <artifactId>c</artifactId>
	      <scope>runtime</scope>
	    </dependency>
	  </dependencies>
	</project>


所有A的管理依赖将被合并加入到B中，除了d，因为它已经在B中定义了。
Project X:

	<project>
	 <modelVersion>4.0.0</modelVersion>
	 <groupId>maven</groupId>
	 <artifactId>X</artifactId>
	 <packaging>pom</packaging>
	 <name>X</name>
	 <version>1.0</version>
	 <dependencyManagement>
	   <dependencies>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>a</artifactId>
	       <version>1.1</version>
	     </dependency>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>b</artifactId>
	       <version>1.0</version>
	       <scope>compile</scope>
	     </dependency>
	   </dependencies>
	 </dependencyManagement>
	</project>

Project Y:

	<project>
	 <modelVersion>4.0.0</modelVersion>
	 <groupId>maven</groupId>
	 <artifactId>Y</artifactId>
	 <packaging>pom</packaging>
	 <name>Y</name>
	 <version>1.0</version>
	 <dependencyManagement>
	   <dependencies>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>a</artifactId>
	       <version>1.2</version>
	     </dependency>
	     <dependency>
	       <groupId>test</groupId>
	       <artifactId>c</artifactId>
	       <version>1.0</version>
	       <scope>compile</scope>
	     </dependency>
	   </dependencies>
	 </dependencyManagement>
	</project>

Project Z:

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>maven</groupId>
	  <artifactId>Z</artifactId>
	  <packaging>pom</packaging>
	  <name>Z</name>
	  <version>1.0</version>
	  <dependencyManagement>
	    <dependencies>
	      <dependency>
	        <groupId>maven</groupId>
	        <artifactId>X</artifactId>
	        <version>1.0</version>
	        <type>pom</type>
	        <scope>import</scope>
	      </dependency>
	      <dependency>
	        <groupId>maven</groupId>
	        <artifactId>Y</artifactId>
	        <version>1.0</version>
	        <type>pom</type>
	        <scope>import</scope>
	      </dependency>
	    </dependencies>
	  </dependencyManagement>
	</project>

Z引用了X和Y的依赖。虽X和Y都包含a，但最终会使用1.1版本，因为1.1版本的a在X中被先声明（先被引用），且Z中也并没有a的声明。
这个引用过程是递归的。比如，如果X引用另外一个pom Q，当Z处理X引用的时候,X已经包含了所有Q的管理依赖；
引用功能相当有效，可以定义一个“library”（一个POM），包含多项目创建的artifact。其他项目就可以很方便的引用这个library中的一个或多个artifact。
但是有时为了使项目中使用的artifact的版本和在library中发布的版本保持一致变得有些困难。以下模式描述一个被其他项目使用的“bill of materials”（BOM）是如何被创建的？
跟项目是BOM pom，它定义了所有在library中创建的artifact的版本。其他项目通过在dependencyManagement中引用这个pom来引用这个library。

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>com.test</groupId>
	  <artifactId>bom</artifactId>
	  <version>1.0.0</version>
	  <packaging>pom</packaging>
	  <properties>
	    <project1Version>1.0.0</project1Version>
	    <project2Version>1.0.0</project2Version>
	  </properties>
	  <dependencyManagement>
	    <dependencies>
	      <dependency>
	        <groupId>com.test</groupId>
	        <artifactId>project1</artifactId>
	        <version>${project1Version}</version>
	      </dependency>
	      <dependency>
	        <groupId>com.test</groupId>
	        <artifactId>project2</artifactId>
	        <version>${project1Version}</version>
	      </dependency>
	    </dependencies>
	  </dependencyManagement>
	  <modules>
	    <module>parent</module>
	  </modules>
	</project>


子项目声明parent为BOM pom。它是一个一般的多项目pom。

	<project >
	  <modelVersion>4.0.0</modelVersion>
	  <parent>
	    <groupId>com.test</groupId>
	    <version>1.0.0</version>
	    <artifactId>bom</artifactId>
	  </parent>
	
	  <groupId>com.test</groupId>
	  <artifactId>parent</artifactId>
	  <version>1.0.0</version>
	  <packaging>pom</packaging>
	
	  <dependencyManagement>
	    <dependencies>
	      <dependency>
	        <groupId>log4j</groupId>
	        <artifactId>log4j</artifactId>
	        <version>1.2.12</version>
	      </dependency>
	      <dependency>
	        <groupId>commons-logging</groupId>
	        <artifactId>commons-logging</artifactId>
	        <version>1.1.1</version>
	      </dependency>
	    </dependencies>
	  </dependencyManagement>
	  <modules>
	    <module>project1</module>
	    <module>project2</module>
	  </modules>
	</project>

接下来，才是实际项目的pom：

	<project >
	  <modelVersion>4.0.0</modelVersion>
	  <parent>
	    <groupId>com.test</groupId>
	    <version>1.0.0</version>
	    <artifactId>parent</artifactId>
	  </parent>
	  <groupId>com.test</groupId>
	  <artifactId>project1</artifactId>
	  <version>${project1Version}</version>
	  <packaging>jar</packaging>
	
	  <dependencies>
	    <dependency>
	      <groupId>log4j</groupId>
	      <artifactId>log4j</artifactId>
	    </dependency>
	  </dependencies>
	</project>



	<project >
	  <modelVersion>4.0.0</modelVersion>
	  <parent>
	    <groupId>com.test</groupId>
	    <version>1.0.0</version>
	    <artifactId>parent</artifactId>
	  </parent>
	  <groupId>com.test</groupId>
	  <artifactId>project2</artifactId>
	  <version>${project2Version}</version>
	  <packaging>jar</packaging>
	
	  <dependencies>
	    <dependency>
	      <groupId>commons-logging</groupId>
	      <artifactId>commons-logging</artifactId>
	    </dependency>
	  </dependencies>
	</project>


以下项目描述了其他项目如何不用声明项目版本（版本信息统一在BOM中定义）而引用到此library。

	<project>
	  <modelVersion>4.0.0</modelVersion>
	  <groupId>com.test</groupId>
	  <artifactId>use</artifactId>
	  <version>1.0.0</version>
	  <packaging>jar</packaging>
	
	  <dependencyManagement>
	    <dependencies>
	      <dependency>
	        <groupId>com.test</groupId>
	        <artifactId>bom</artifactId>
	        <version>1.0.0</version>
	        <type>pom</type>
	        <scope>import</scope>
	      </dependency>
	    </dependencies>
	  </dependencyManagement>
	  <dependencies>
	    <dependency>
	      <groupId>com.test</groupId>
	      <artifactId>project1</artifactId>
	    </dependency>
	    <dependency>
	      <groupId>com.test</groupId>
	      <artifactId>project2</artifactId>
	    </dependency>
	  </dependencies>
	</project>


最后,创建项目引用依赖的时候需要注意：

1. 不要引用一个定义在当前pom的一个子模块中的pom，如果那样做会出现找不到pom的错误；
2. 不要定义一个pom A引用另外一个pom B,且A作为B的父类（或祖父类等），因为没有办法解决循环引用而会抛出一个错误。
3. 当所依赖的artifacts所属的pom有传递依赖的时候，项目需要声明这些artifacts的版本以使之成为可控的依赖。不这样做会导致build失败，因为这些artifact可能没有声明版本信息。（在任何情况下这都可以被认为是最佳实践，因为从一次build到下次build它能始终保持一致的版本信息）

<h3><b>9.系统依赖(System Dependencies)</b></h3>
声明system范围的依赖是任何时候都可以引用到，不需要从repository中查找。它经常用来告知maven这个依赖是有JDK或VM提供的。所以，系统依赖对解决由JDK提供的artifact，它们可以提前下载好。如JDBC、JAAS（Java Authentication and Authorization Service）等。
一个简单的实例如：

	<project>
	  ...
	  <dependencies>
	    <dependency>
	      <groupId>javax.sql</groupId>
	      <artifactId>jdbc-stdext</artifactId>
	      <version>2.0</version>
	      <scope>system</scope>
	      <systemPath>${java.home}/lib/rt.jar</systemPath>
	    </dependency>
	  </dependencies>
	  ...
	</project>


如果你的artifact由JDK的tools.jar提供可以像如下方式定义：

	<project>
	  ...
	  <dependencies>
	    <dependency>
	      <groupId>sun.jdk</groupId>
	      <artifactId>tools</artifactId>
	      <version>1.5.0</version>
	      <scope>system</scope>
	      <systemPath>${java.home}/../lib/tools.jar</systemPath>
	    </dependency>
	  </dependencies>
	  ...
	</project>


其他：
1.从Maven项目中导出项目依赖的jar包：

	mvn dependency:copy-dependencies

参考：<a href="http://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html">Introduction to the Dependency Mechanism</a>

<h3><b>10. FAQs</b></h3>
<strong>FAQ1: What’s maven artifact?</strong>

An artifact is a file, usually a JAR, that gets deployed to a Maven repository.

A Maven build produces one or more artifacts, such as a compiled JAR and a "sources" JAR.

Each artifact has a group ID (usually a reversed domain name, like com.example.foo), an artifact ID (just a name), and a version string. The three together uniquely identify the artifact.

A project's dependencies are specified as artifacts.

参考：<a href="http://stackoverflow.com/questions/2487485/what-is-maven-artifact">source</a>

<strong>FAQ2: What exactly is a Maven Snapshot and why do we need it?</strong>

A snapshot version in Maven is one that has not been released.

The idea is that before a 1.0 release (or any other release) is done, there exists a 1.0-SNAPSHOT. That version is what might become 1.0. It's basically "1.0 under development". This might be close to a real 1.0 release, or pretty far (right after the 0.9 release, for example).

The difference between a "real" version an a snapshot version is that snapshots might get updates. That means that downloading 1.0-SNAPSHOT today might give a different file than downloading it yesterday.

Usually, snapshot dependencies should only exist during development and no released version (i.e. no non-snapshot) should have a dependency on a snapshot version.

he three others answers provide you a good vision of what a -SNAPSHOT version is. I just wanted to add some information regarding the behavior of Maven when it finds a SNAPSHOT dependency.

When you build an application, Maven will search for dependencies in the local repository. If a stable version is not found there, it will search the remote repositories (defined in settings.xml or pom.xml) to retrieve this dependency. Then, it will copy it into the local repository, to make it available for the next builds.

For example, a foo-1.0.jar library is considered as a stable version, and if Maven finds it in the local repository, it will use this one for the current build.

Now, if you need a foo-1.0-SNAPSHOT.jar library, Maven will know that this version is not stable and is subject to changes. That's why Maven will try to find a newer version in the remote repositories, even if a version of this library is found on the local repository. However, this check is made only once per day. That means that if you have a foo-1.0-20110506.110000-1.jar (i.e. this library has been generated on 2011/05/06 at 11:00:00) in your local repository, and if you run the Maven build again the same day, Maven will not check the repositories for a newer version.

Maven provides you a way to can change this update policy in your repository definition:

	<repository>
	    <id>foo-repository</id>
	    <url>...</url>
	    <snapshots>
	        <enabled>true</enabled>
	        <updatePolicy>XXX</updatePolicy>
	    </snapshots>
	</repository>


where XXX can be:

* always: Maven will check for a newer version on every build;
* daily, the default value;
* interval:XXX: an interval in minutes (XXX)
* never: Maven will never try to retrieve another version. It will do that only if it doesn't exist locally. With the configuration, SNAPSHOT version will be handled as the stable libraries.
(model of the settings.xml can be found here)

参考：<a href="http://stackoverflow.com/questions/5901378/what-exactly-is-a-maven-snapshot-and-why-do-we-need-it">source</a>