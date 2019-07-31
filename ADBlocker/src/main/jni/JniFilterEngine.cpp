/*
 * This file is part of Adblock Plus <https://adblockplus.org/>,
 * Copyright (C) 2006-2015 Eyeo GmbH
 *
 * Adblock Plus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * Adblock Plus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Adblock Plus.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <AdblockPlus.h>
#include "Utils.h"
#include "JniJsValue.h"

static jobject SubscriptionsToArrayList(JNIEnv* env, std::vector<AdblockPlus::SubscriptionPtr>& subscriptions)
{
  jobject list = NewJniArrayList(env);

  for (std::vector<AdblockPlus::SubscriptionPtr>::iterator it = subscriptions.begin(), end = subscriptions.end(); it != end; it++)
  {
    JniAddObjectToList(env, list, NewJniSubscription(env, *it));
  }

  return list;
}

static AdblockPlus::FilterEngine::ContentType ConvertContentType(JNIEnv *env,
    jobject jContentType)
{
  JniLocalReference<jclass> contentTypeClass(env,
      env->GetObjectClass(jContentType));
  jmethodID nameMethod = env->GetMethodID(*contentTypeClass, "name",
      "()Ljava/lang/String;");
  JniLocalReference<jstring> jValue(env,
      (jstring) env->CallObjectMethod(jContentType, nameMethod));
  const std::string value = JniJavaToStdString(env, *jValue);
  return AdblockPlus::FilterEngine::StringToContentType(value);
}

static jlong JNICALL JniCtor(JNIEnv* env, jclass clazz, jlong enginePtr)
{
  try
  {
    AdblockPlus::JsEnginePtr& jsEngine = *JniLongToTypePtr<AdblockPlus::JsEnginePtr>(enginePtr);
    return JniPtrToLong(new AdblockPlus::FilterEngine(jsEngine));
  }
  CATCH_THROW_AND_RETURN(env, 0)
}

static void JNICALL JniDtor(JNIEnv* env, jclass clazz, jlong ptr)
{
  delete JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);
}

static jboolean JNICALL JniIsFirstRun(JNIEnv* env, jclass clazz, jlong ptr)
{
  try
  {
    AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

    return engine->IsFirstRun() ? JNI_TRUE : JNI_FALSE;
  }
  CATCH_THROW_AND_RETURN(env, JNI_FALSE);
}

static jobject JNICALL JniGetFilter(JNIEnv* env, jclass clazz, jlong ptr, jstring jText)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);
  std::string text = JniJavaToStdString(env, jText);

  try
  {
    AdblockPlus::FilterPtr filter = engine->GetFilter(text);

    return NewJniFilter(env, filter);
  }
  CATCH_THROW_AND_RETURN(env, 0);
}

static jobject JNICALL JniGetListedFilters(JNIEnv* env, jclass clazz, jlong ptr)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  try
  {
    std::vector<AdblockPlus::FilterPtr> filters = engine->GetListedFilters();

    jobject list = NewJniArrayList(env);

    for (std::vector<AdblockPlus::FilterPtr>::iterator it = filters.begin(), end = filters.end(); it != end; it++)
    {
      JniAddObjectToList(env, list, *JniLocalReference<jobject>(env, NewJniFilter(env, *it)));
    }

    return list;
  }
  CATCH_THROW_AND_RETURN(env, 0);
}

static jobject JNICALL JniGetSubscription(JNIEnv* env, jclass clazz, jlong ptr, jstring jUrl)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);
  std::string url = JniJavaToStdString(env, jUrl);

  try
  {
    AdblockPlus::SubscriptionPtr subscription = engine->GetSubscription(url);

    return NewJniSubscription(env, subscription);
  }
  CATCH_THROW_AND_RETURN(env, 0);
}

static jobject JNICALL JniGetListedSubscriptions(JNIEnv* env, jclass clazz, jlong ptr)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  try
  {
    std::vector<AdblockPlus::SubscriptionPtr> subscriptions = engine->GetListedSubscriptions();

    return SubscriptionsToArrayList(env, subscriptions);
  }
  CATCH_THROW_AND_RETURN(env, 0);
}

static jobject JNICALL JniGetElementHidingSelectors(JNIEnv* env, jclass clazz,
    jlong ptr, jstring jDomain)
{
  AdblockPlus::FilterEngine* engine =
      JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  std::string domain = JniJavaToStdString(env, jDomain);

  try
  {
    std::vector<std::string> selectors = engine->GetElementHidingSelectors(
        domain);

    jobject list = NewJniArrayList(env);

    for (std::vector<std::string>::iterator it = selectors.begin(), end =
        selectors.end(); it != end; it++)
    {
      JniAddObjectToList(env, list,
          *JniLocalReference<jstring>(env, env->NewStringUTF(it->c_str())));
    }

    return list;
  }
  CATCH_THROW_AND_RETURN(env, 0)
}

static jobject JNICALL JniMatches(JNIEnv* env, jclass clazz, jlong ptr, jstring jUrl, jobject jContentType, jstring jDocumentUrl)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  std::string url = JniJavaToStdString(env, jUrl);
  AdblockPlus::FilterEngine::ContentType contentType =
      ConvertContentType(env, jContentType);
  std::string documentUrl = JniJavaToStdString(env, jDocumentUrl);

  try
  {
    AdblockPlus::FilterPtr filter = engine->Matches(url, contentType, documentUrl);

    return NewJniFilter(env, filter);
  }
  CATCH_THROW_AND_RETURN(env, 0)
}

static jobject JNICALL JniMatchesMany(JNIEnv* env, jclass clazz, jlong ptr,
    jstring jUrl, jobject jContentType, jobjectArray jDocumentUrls)
{
  AdblockPlus::FilterEngine* engine =
      JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  std::string url = JniJavaToStdString(env, jUrl);
  AdblockPlus::FilterEngine::ContentType contentType =
      ConvertContentType(env, jContentType);

  std::vector<std::string> documentUrls;

  jsize len = env->GetArrayLength(jDocumentUrls);

  for (jsize i = 0; i < len; i++)
  {
    documentUrls.push_back(
        JniJavaToStdString(env,
            *JniLocalReference<jstring>(env,
                static_cast<jstring>(env->GetObjectArrayElement(jDocumentUrls,
                    i)))));
  }

  try
  {
    AdblockPlus::FilterPtr filter = engine->Matches(url, contentType,
        documentUrls);

    return NewJniFilter(env, filter);
  }
  CATCH_THROW_AND_RETURN(env, 0)
}

static jobject JNICALL JniGetPref(JNIEnv* env, jclass clazz, jlong ptr, jstring jPref)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr < AdblockPlus::FilterEngine > (ptr);

  std::string pref = JniJavaToStdString(env, jPref);

  try
  {
    AdblockPlus::JsValuePtr value = engine->GetPref(pref);

    return NewJniJsValue(env, value);
  }
  CATCH_THROW_AND_RETURN(env, 0)
}

static void JNICALL JniSetPref(JNIEnv* env, jclass clazz, jlong ptr, jstring jPref, jlong jsValue)
{
  AdblockPlus::FilterEngine* engine = JniLongToTypePtr<AdblockPlus::FilterEngine>(ptr);

  std::string pref = JniJavaToStdString(env, jPref);
  AdblockPlus::JsValuePtr value = JniGetJsValuePtr(jsValue);

  try
  {
    engine->SetPref(pref, value);
  }
  CATCH_AND_THROW(env)
}

static JNINativeMethod methods[] =
{
  { (char*)"ctor", (char*)"(J)J", (void*)JniCtor },
  { (char*)"isFirstRun", (char*)"(J)Z", (void*)JniIsFirstRun },
  { (char*)"getFilter", (char*)"(JLjava/lang/String;)" TYP("Filter"), (void*)JniGetFilter },
  { (char*)"getListedFilters", (char*)"(J)Ljava/util/List;", (void*)JniGetListedFilters },
  { (char*)"getSubscription", (char*)"(JLjava/lang/String;)" TYP("Subscription"), (void*)JniGetSubscription },
  { (char*)"getListedSubscriptions", (char*)"(J)Ljava/util/List;", (void*)JniGetListedSubscriptions },
  { (char*)"getElementHidingSelectors", (char*)"(JLjava/lang/String;)Ljava/util/List;", (void*)JniGetElementHidingSelectors },
  { (char*)"matches", (char*)"(JLjava/lang/String;" TYP("FilterEngine$ContentType") "Ljava/lang/String;)" TYP("Filter"), (void*)JniMatches },
  { (char*)"matches", (char*)"(JLjava/lang/String;" TYP("FilterEngine$ContentType") "[Ljava/lang/String;)" TYP("Filter"), (void*)JniMatchesMany },
  { (char*)"getPref", (char*)"(JLjava/lang/String;)" TYP("JsValue"), (void*)JniGetPref },
  { (char*)"setPref", (char*)"(JLjava/lang/String;J)V", (void*)JniSetPref },
  { (char*)"dtor", (char*)"(J)V", (void*)JniDtor }
};

JAVA_PACKAGE_REGISTERNATIVES(FilterEngine)
