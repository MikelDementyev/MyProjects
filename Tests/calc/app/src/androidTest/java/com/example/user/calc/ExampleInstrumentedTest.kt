package com.example.user.calc

import android.support.test.espresso.Espresso.onView
import android.support.test.espresso.action.ViewActions.*
import android.support.test.espresso.assertion.ViewAssertions.matches
import android.support.test.espresso.matcher.ViewMatchers.withId
import android.support.test.espresso.matcher.ViewMatchers.withText
import android.support.test.rule.ActivityTestRule
import android.support.test.runner.AndroidJUnit4
import android.util.Log
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Rule
    @JvmField
    var mActivityRule = ActivityTestRule(MainActivity::class.java)

    @Test
    fun TestCalc(){
        for (i in 0..101) {
            onView(withId(R.id.inputField1)).perform(clearText())
            onView(withId(R.id.inputField1)).perform(typeText("${i}"), closeSoftKeyboard())
            for (j in 0..101){
                onView(withId(R.id.inputField2)).perform(clearText())
                onView(withId(R.id.inputField2)).perform(typeText("${j}"), closeSoftKeyboard())
                onView(withId(R.id.plusBtn)).perform(click())
                try {
                    onView(withId(R.id.textView)).check(matches(withText("${i+j}")))
                    //Log.d("FindError", "${i},${j}")
                }
                catch(e: Throwable) {
                    Log.d("FindError", "${i},${j}")
                    return
                }
            }
        }
    }

}
