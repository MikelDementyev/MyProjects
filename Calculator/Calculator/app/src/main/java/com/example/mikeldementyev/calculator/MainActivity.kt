package com.example.mikeldementyev.calculator

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*
import java.lang.Double.parseDouble
import java.lang.Long.parseLong

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        var flag = false

        fun IsNumber(a: String, b: String): Boolean {
//      return (a.matches("-?\\d+(\\.\\d+)?".toRegex()) && b.matches("-?\\d+(\\.\\d+)?".toRegex()))
            try {
                parseDouble(a)
                parseDouble(b)
                return true
            } catch (e: NumberFormatException) {
                return false
            }
        }

        fun IsHexadecimal(a: String?, b: String?): Boolean {
            try {
                parseLong(a, 16)
                parseLong(b, 16)
                return true
            } catch (e: NumberFormatException) {
                return false
            }
        }

        fun FirstNumber(): String {
            return FirstArg.text.toString()
        }

        fun SecondNumber(): String {
            return SecondArg.text.toString()
        }

        Foundations.setOnCheckedChangeListener { _, isOn ->
            flag = isOn
        }

        val click = View.OnClickListener { Sign ->

            val a = FirstNumber()
            val b = SecondNumber()
            if (IsNumber(a, b) && !flag) {
                when (Sign) {
                    buttonPlus -> AnswerView.text = (a.toDouble() + b.toDouble()).toString()
                    buttonMinus -> AnswerView.text = (a.toDouble() - b.toDouble()).toString()
                    buttonMultiply -> AnswerView.text = (a.toDouble() * b.toDouble()).toString()
                    buttonDivide -> AnswerView.text = (a.toDouble() / b.toDouble()).toString()
                }
            } else {
                if (!flag) {
                    AnswerView.text = "Format error"
                } else {
                    if (IsHexadecimal(a, b)) {
                        when (Sign) {
                            buttonPlus -> AnswerView.text = (a.toLong(16) + b.toLong(16)).toString(16)
                            buttonMinus -> AnswerView.text = (a.toLong(16) - b.toLong(16)).toString(16)
                            buttonMultiply -> AnswerView.text = (a.toLong(16) * b.toLong(16)).toString(16)
                            buttonDivide -> AnswerView.text = (a.toLong(16) / b.toLong(16)).toString(16)
                        }
                    } else AnswerView.text = "Format error"
                }
            }
        }

        buttonPlus.setOnClickListener(click)
        buttonMinus.setOnClickListener(click)
        buttonMultiply.setOnClickListener(click)
        buttonDivide.setOnClickListener(click)

    }
}