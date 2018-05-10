package com.example.mikeldementyev.unittests

import org.junit.Assert.assertEquals
import org.junit.Test
import kotlin.math.sqrt

fun LongOfStr(str: String):Int{
    var long = 0;
    for (i in str){
        long += 1
    }
    return long
}

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        assertEquals(4, 2 + 2)
    }

    @Test
    fun TestSqrt(){
        assertEquals(42, sqrt(1764.0).toInt())
    }

    @Test
    fun MeasurementError1(){
        var dx = 2.5
        assertEquals(sqrt(dx + 1.0), 1 + 0.5*dx, 0.5 )
    }
    @Test
    fun MeasureError2() {
        var dx = 10.0
        assertEquals(sqrt(dx + 1.0), 1 + 0.5 * dx, 0.5)
    }
    @Test
    fun MeasureError3() {
        var dx = 25.0
        assertEquals(sqrt(dx + 1.0), 1 + 0.5 * dx, 0.5)
    }

    @Test
    fun TestLength(){
        var str = "Very Long STR"
        assertEquals(LongOfStr(str), str.length)
    }
}


