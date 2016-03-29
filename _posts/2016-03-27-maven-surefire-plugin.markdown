---
author: gelnyang
comments: true
layout: post
title: Maven Surefire Plugin
categories:
- Knowledge
- Collection
tags:
- maven
- surefire
- test
---
#Introduction
-----------------------------
surefire

	adj. 准不会有错的，一定成功的

The Surefire Plugin is used during the 【test phase】 of the build lifecycle to execute the unit tests of an application. 
It generates reports in two different file formats:

* Plain text files (*.txt)
* XML files (*.xml)

By default, these files are generated at ${basedir}/target/surefire-reports.


#Plguin Goals
-----------------------------
The Surefire Plugin has only one goal:

	surefire:test 
		runs the unit tests of an application.

#Test Single Class
-----------------------------
Use parameter test:
 
	<plugin>
		<groupId>org.apache.maven.plugins</groupId>
		<artifactId>maven-surefire-plugin</artifactId>
		<version>2.19.1</version>
		<configuration>
			<test>FooTest</test>
		</configuration>
	</plugin>

Or, provide the parameter from command line:

	mvn test -Dtest=FooTest

#Skip Test
-----------------------------
Use parameter skipTests:

	<plugin>
		<groupId>org.apache.maven.plugins</groupId>
		<artifactId>maven-surefire-plugin</artifactId>
		<version>2.19.1</version>
		<configuration>
			<skipTests>true</skipTests>
		</configuration>
	</plugin>

Or, provide the parameter from command line:

	mvn install -DskipTests=true

#Using JUnit Categories to Limit Test Scope
-----------------------------
JUnit 4.8 introduced the notion of Categories. You can use JUnit categories by using the groups parameter.

	<plugins>
	    <plugin>
	      <artifactId>maven-surefire-plugin</artifactId>
	      <version>2.11</version>
	      <configuration>
	        <groups>com.mycompany.BuildTest</groups>
	      </configuration>
	    </plugin>
	</plugins>

This will execute only those tests annotated with the @Category(com.mycompany.SlowTests.class) annotation and those tests annotated with @Category(com.mycompany.SlowerTests.class) if class/interface SlowerTests is subclass of SlowTests:

    public interface SlowTests{}
    public interface SlowerTests extends SlowTests{}
    public class AppTest {
      @Test
      @Category(com.mycompany.BuildTest.class)
      public void testSlow() {
        System.out.println("slow");
      }
    }
The @Category annotation can also be applied at [class-level].

See [Surefire Junit]

#Test Parallel
-----------------------------

Simply, using two parameter to control parallel:

* parallel
   
	possible values: methods, classes, both, suites, suitesAndClasses, suitesAndMethods, classesAndMethods or all

* threadCount


[NOTICE] DBUnit testes always have db data initialization before every test method, the following situations may cause data confilict:

* configuring parallel in methods mode and there are more than one method in one class.
* configuring parallel in classes mode and there are tables initialized by more than one test classes.

Run these testes which may cause conflict in single thread can resolve this issue. Use @NotThreadSafe to annotation these testes.

Add jcip dependency in POM first:

	<dependency>
		<groupId>net.jcip</groupId>
		<artifactId>jcip-annotations</artifactId>
		<version>1.0</version>
		<scope>test</scope>
	</dependency>

Add @NotThreadSafe annotation to test class:

	@NotThreadSafe
	public class FooTest  {
	}

#Multiple Test JVM Instances
-----------------------------
The parameter forkCount defines the maximum number of JVM processes that Surefire will spawn concurrently to execute the tests. 

The parameter reuseForks is used to define whether to terminate the spawned process after one test class and to create a new process for the next test in line (reuseForks=false), or whether to reuse the processes to execute the next tests (reuseForks=true).

With the argLine property, you can specify additional parameters to be passed to the forked JVM process, such as memory settings. 

	<plugins>
	    <plugin>
	      <artifactId>maven-surefire-plugin</artifactId>
	      <version>2.11</version>
	      <configuration>
	        <forkCount>3</forkCount>
	        <reuseForks>true</reuseForks>
	        <argLine>-Xmx1024m -XX:MaxPermSize=256m</argLine>
	      </configuration>
	    </plugin>
	</plugins>

#Configuration Example
-----------------------------
	<build>
		<plugins>
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-surefire-plugin</artifactId>
				<version>2.19.1</version>
				<configuration>
	        		<groups>com.mycompany.BuildTest</groups>
					<parallel>classes</parallel>
          			<threadCount>10</threadCount>
			        <forkCount>3</forkCount>
			        <reuseForks>true</reuseForks>
			        <argLine>-Xmx1024m -XX:MaxPermSize=256m</argLine>
				</configuration>
			</plugin>
		</plugins>
	</build>



------------------
[Reference]

[reference]: https://maven.apache.org/surefire/maven-surefire-plugin/
[Surefire Junit]: https://maven.apache.org/surefire/maven-surefire-plugin/examples/junit.html