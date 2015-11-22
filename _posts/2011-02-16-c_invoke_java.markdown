---
author: admin
comments: true
date: 2011-02-16 02:36:42+00:00
excerpt: c调用java的例子
layout: post
slug: c_invoke_java
title: C调用Java的例子
wordpress_id: 63
categories:
- Experience
tags:
- C
- java
- jni
---

How to run this example:
1. exec command "javac jni/CJava.java" to complie the java code.
2. add the two direcotories JAVA_HOME/include and JAVA_HOME/include/win32(under windows) as include files.
3. add JAVA_HOME/lib as library files.
4. add JAVA_HOME/lib/jvm.lib into project.
5. complie and exec this program.

Note:
1. You must have installed JDK 1.6 and configured the environment variables JAVA_HOME and PATH for java.
2. exec command "javap -s -p jni.CJava" to view the signatures of methods.


CJava.java:

    package jni;
    public class CJava {
    	public static String getstring(String str) {
    		return str + " ---> added";
    	}
    	public static int add(int a, int b) {
    		return a + b;
    	}
    	public int sub(int a, int b) {
    		return a - b;
    	}
    	public static void main(String[] args) {
    		System.out.println("Hello world!");
    	}
    }



cjava.c:

    #include 
    #include 
    #include 
    
    int main()
    {
    	JavaVMOption options[1];
    	JNIEnv *env;
    	JavaVM *jvm;
    	JavaVMInitArgs vm_args;
    	long status;
    	jclass cls;
    	jmethodID mid;
    
    	char* class_name="jni/CJava";
    
    	options[0].optionString = "-Djava.class.path=.";
    	vm_args.version = JNI_VERSION_1_6;
    	vm_args.nOptions = 1;
    	vm_args.options = options;
    	vm_args.ignoreUnrecognized = JNI_TRUE;
    
    	/* Create the Java VM */
    	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    	if (status < 0 || status == JNI_ERR) {
    		printf("Status of creating Java VM:%d\n",status);
    		fprintf(stderr, "Failed to create Java VM!\n");
    		return 1;
    	}
    
    	/*Find the class obj*/
    	cls = (*env)->FindClass(env, class_name);
    	if(cls !=0)
    	{
    		/* ---------------------------------------------------------- */
    		/*Test to invoke a static method*/
    
    		char* static_method_name="add";
    		char* static_method_sign="(II)I";
    
    		/*Get method ID*/
    		mid = (*env)->GetStaticMethodID(env, cls, static_method_name, static_method_sign);
    		if(mid !=0)
    		{
    			/*invoke static int method*/
    			jint result = (*env)->CallStaticIntMethod(env, cls, mid, 5,1);
    			printf("Call static method %s: %d\n",static_method_name,result);
    		}else{
    			printf("Failed to find method %s!\n",static_method_name);
    		}
    		/* ---------------------------------------------------------- */
    		/*Test to invoke a instance method*/
    		char* instance_method_name="sub";
    		char* instance_method_sign="(II)I";
    
    		jmethodID constructor_mid = (*env)->GetMethodID(env,cls,"","()V"); //The construcotr method name is 
    		jobject    jobj = (*env)->NewObject(env,cls,constructor_mid);
    
    		/*Get method ID*/
    		mid = (*env)->GetMethodID(env,cls,instance_method_name,instance_method_sign);
    		if(mid !=0)
    		{
    			/*invoke int method*/
    			jint result = (*env)->CallIntMethod(env, jobj, mid, 9,4);
    			printf("Call instance method %s: %d\n",instance_method_name,result);
    		}else{
    			printf("Failed to find method %s!\n",instance_method_name);
    		}
    	}else{
    		printf("Failed to find Class %s!\n",class_name);
    	}
    
    	/*destory JVM*/
    	(*jvm)->DestroyJavaVM(jvm);
    	return 0;
    }
    

-------------------------------

20140925 Test on Redhat Linux 64bit:

    > export JAVA_HOME=/usr/java/jdk1.7.0_45
    > export PATH=$JAVA_HOME/bin:$PATH
    > export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64/server/:$LD_LIBRARY_PATH
    > javac jni/CJava.java
    > gcc -o cjava -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -L$JAVA_HOME/bin -L$JAVA_HOME/jre/lib/amd64/server  cjava.c -ljvm
    > ./cjava 
    Call static method add: 6
    Call instance method sub: 5


