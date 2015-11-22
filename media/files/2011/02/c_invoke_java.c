/************************************************************************
The following is the Java code:
===============================
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
===============================

How to run this example:
1. exec command "javac jni/CJava.java" to complie the java code.
2. add the two direcotories JAVA_HOME/include and JAVA_HOME/include/win32(under windows) as include files.
3. add JAVA_HOME/lib as library files.
4. add JAVA_HOME/lib/jvm.lib into project.
5. complie and exec this program.

Note:
1. You must have installed JDK 1.6 and configured the environment variables JAVA_HOME and PATH for java.
2. exec command "javap -s -p jni.CJava" to view the signatures of methods.

Author:Geln Yang
Mail:gelnyang@gmail.com
*************************************************************************/

#include <stdio.h>
#include <tchar.h>
#include <jni.h>
#include <stdarg.h>

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
	    exit(1);
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
		
		jmethodID constructor_mid = (*env)->GetMethodID(env,cls,"<init>","()V"); //The construcotr method name is <init>
		jobject	jobj = (*env)->NewObject(env,cls,constructor_mid);

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
