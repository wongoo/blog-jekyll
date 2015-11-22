---
author: admin
comments: true
date: 2015-07-11 17:03:17+00:00
excerpt: Call C API through Java Native Interfaces
layout: post
slug: call-c-api-through-java-native-interfaces
title: Call C API through Java Native Interfaces
wordpress_id: 870
categories:
- Experience
tags:
- C
- java
- jni
---

Java could call C API through Java Native Interfaces, we could implement that in following steps:
There should be a diagram to illustrate this process:
 
JAVA program --> JNI class --> .so/.dll (implement c API generated from JNI class) --> C APIs we want to call

==========================================

1. Build a Java class, declaring the native interfaces. These native interfaces are called by other classes who want to call the C APIs.

a example of JNI class:

    package coresystem; 
    class Adapter { 
    	native String call(String in); 
    	static {
    		System.loadLibrary("coresystem"); 
    	}
    }
	
Note:
a. We must declare all the APIs which we want to call them in java into native methods;
b. Native methods are loaded with the System.loadLibrary method. The argument to System.loadLibrary is a library name chosen arbitrarily by the programmer. The system follows a standard, but platform-specific, approach to convert the library name to a native library name. For example, a Solaris system converts the name coresystem to libcoresystem.so, while a Win32 system converts the same coresystem name to coresystem.dll.

2. Compiling:

creating class file:

    javac coresystem/Adapter.java
	
creating head file for native library:

    javah coresystem.Adapter
	
You could see a file named coresystem_Adapter.h was generated.

3. implement the C APIs descripted in coresystem_Adapter.h and create a .so file. 
In this .so file, there should be the logic to call the C APIs that Java want to call.

    export JAVA_HOME=/usr/java/jdk1.7.0_45
    export PATH=$JAVA_HOME/bin:$PATH
    export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64/server/:$LD_LIBRARY_PATH
    export C_INCLUDE_PATH=$JAVA_HOME/include:$JAVA_HOME/include/linux
    gcc -g coresystem.c -o libcoresystem.so  -shared
	
4. set environment for the new generated .so:

    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/jc

5. Run Java program, it works:

    package coresystem; 
    public class Test { 
    	public static final void main(String[] args){
    		Adapter adapter = new Adapter();
    		String response = adapter.call(args[0]);
    		System.out.println(response);
    	}
    }

complie and run:

    javac coresystem/Test.java
    java coresystem.Test hello

