package MyProject.DefOfDet

import java.util.Random

val random = Random()

const val MinValOfEl = 0
const val MaxValOfEl = 10

fun rand(from: Int, to: Int) : Int {
    return random.nextInt(to - from) + from
}

fun main(args: Array<String>){
    print("Введите порядок генерируемой матрицы: ")
    val n = readLine()
    if (n == null) return
    val nI = n.toInt()
    print("Матрица в формате <a11 a12;a21 a22>: ")
    for(i in 0 until nI){
        for(j in 0 until nI){
            if (j == nI-1){
                print("${rand(MinValOfEl, MaxValOfEl)}")
            }else {
                print("${rand(MinValOfEl, MaxValOfEl)} ")
            }
        }
        if (i != nI-1) {
            print(";")
        }
    }
}

