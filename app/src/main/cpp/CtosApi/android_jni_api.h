/*
 * Android JNI API
 */

#ifndef _ANDROID_API_PRINT_H_
#define _ANDROID_API_PRINT_H_

#include <jni.h>
#include <stddef.h>
#include <android_jni_log.h>

#define SIZE_OF_ARRAY(ary) sizeof(ary) / sizeof(*ary)

char* get_jstring(JNIEnv* env, jobject *obj, char *name) {

    jclass obj_class = (*env)->GetObjectClass(env, *obj);

    jfieldID get_str_id = (*env)->GetFieldID(env, obj_class, name, "Ljava/lang/String;");
    jbyteArray get_str_fd = (*env)->GetObjectField(env, *obj, get_str_id);
    const char *str_j = (*env)->GetStringUTFChars(env, get_str_fd, NULL);

    return (char*)str_j;
}

void get_jbyteArray(JNIEnv* env, jobject *obj, jbyte *arr, char *name) {

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[B");
    jbyteArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jbyte *get_arr_val = (*env)->GetByteArrayElements(env, get_arr_fd, NULL);

    arr = get_arr_val;
    (*env)->ReleaseByteArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void get_jintArray(JNIEnv* env, jobject *obj, jint *arr, char *name) {

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[I");
    jintArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jint *get_arr_val = (*env)->GetIntArrayElements(env, get_arr_fd, NULL);

    arr = get_arr_val;
    (*env)->ReleaseByteArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void get_jcharArray(JNIEnv* env, jobject *obj, jchar *arr, char *name) {

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[I");
    jcharArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jchar *get_arr_val = (*env)->GetCharArrayElements(env, get_arr_fd, NULL);

    arr = get_arr_val;
    (*env)->ReleaseByteArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void get_jbyte(JNIEnv* env, jobject *obj, jbyte *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "B");
	jbyte get_val = (*env)->GetByteField(env, *obj, get_id);
	LOGE("get_jbyte_val = %d\n", get_val);
	*val = get_val;
}

void get_jchar(JNIEnv* env, jobject *obj, jchar *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "C");
	jbyte get_val = (*env)->GetCharField(env, *obj, get_id);
	LOGE("get_jchar_val = %d\n", get_val);
	*val = get_val;
}

void get_jint(JNIEnv* env, jobject *obj, jint *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "I");
	jbyte get_val = (*env)->GetIntField(env, *obj, get_id);
	LOGE("get_jint_val = %d\n", get_val);
	*val = get_val;
}

void set_jbyteArray(JNIEnv* env, jobject *obj, jbyte *arr, void* len, char *name) {
	jchar num = 0, len_c;
	len_c = (jchar*)len;

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[B");
    jbyteArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jbyte *get_arr_val = (*env)->GetByteArrayElements(env, get_arr_fd, NULL);

    LOGE("byte len = %d\n", len_c);
	for(num = 0 ; num < len_c ; num++) {
		get_arr_val[num] = arr[num];
	}

    (*env)->ReleaseByteArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void set_jintArray(JNIEnv* env, jobject *obj, jint *arr, void* len, char *name) {
	jchar num = 0, len_c;
	len_c = (jchar*)len;

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[I");
    jintArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jint *get_arr_val = (*env)->GetIntArrayElements(env, get_arr_fd, NULL);

    LOGE("int len = %d\n", len_c);
	for(num = 0 ; num < len_c ; num++) {
		get_arr_val[num] = arr[num];
	}


    (*env)->ReleaseIntArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void set_jcharArray(JNIEnv* env, jobject *obj, jchar *arr, void* len, char *name) {
	jchar num = 0, len_c;
	len_c = (jchar*)len;

    jclass obj_class = (*env)->GetObjectClass(env, *obj);
    jfieldID get_arr_id = (*env)->GetFieldID(env, obj_class, name, "[I");
    jcharArray get_arr_fd = (*env)->GetObjectField(env, *obj, get_arr_id);
    jchar *get_arr_val = (*env)->GetCharArrayElements(env, get_arr_fd, NULL);

    LOGE("char len = %d\n", len_c);
	for(num = 0 ; num < len_c ; num++) {
		get_arr_val[num] = arr[num];
	}

    (*env)->ReleaseCharArrayElements(env, get_arr_fd, get_arr_val, NULL);
}

void set_jbyte(JNIEnv* env, jobject *obj, jbyte *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "B");
	(*env)->SetByteField(env, *obj, get_id, *val);
	LOGE("set_jbyte_val = %d\n", *val);
}

void set_jchar(JNIEnv* env, jobject *obj, jchar *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "C");
	(*env)->SetCharField(env, *obj, get_id, *val);
	LOGE("set_jchar_val = %d\n", *val);
}

void set_jint(JNIEnv* env, jobject *obj, jint *val, char *name) {
	jclass obj_class = (*env)->GetObjectClass(env, *obj);
	jfieldID get_id = (*env)->GetFieldID(env, obj_class, name, "I");
	(*env)->SetIntField(env, *obj, get_id, *val);
	LOGE("set_jint_val = %d\n", *val);
}

#endif
