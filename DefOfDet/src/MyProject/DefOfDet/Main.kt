package MyProject.DefOfDet

var indicator = 1.0

fun main(args: Array<String>) {
    var index = 0
    print("Введите матрицу в формате <a11 a12;a21 a22>: ")
    val str: String? = readLine()
    if (str == null) return
    val ptr_str = str.split(" |;".toRegex())
    var count = ptr_str.size.toDouble()
    count = Math.sqrt(count)
    val countI = count.toInt()
    val matrix: Array<Array<Double>> = Array(countI, { Array(countI, { 0.0 }) })
    for (i in 0 until countI) {
        for (j in 0 until countI) {
            matrix[i][j] = (ptr_str[index]).toDouble()
            index += 1
        }
    }
    println("Прогресс выполнения программы: ")
    print(">0%                           |<<")
    val det = searchDet(matrix, numbOfIt(matrix))
    println()
    print("Определитель матрицы равен: ${det}")
}

fun delColRow(matrix: Array<Array<Double>>, rowForDel: Int, colForDel: Int, newMatrix: Array<Array<Double>>){
    var delRow = 0
    for(i in 0 until matrix.size-1){
        if(i == rowForDel){
            delRow = 1
        }
        var delCol = 0
        for(j in 0..matrix.size-2){
            if(j == colForDel){
                delCol = 1
            }
            newMatrix[i][j] = matrix[i + delRow][j + delCol]
        }
    }
}

fun numbOfIt(matrix: Array<Array<Double>>):Double {
    var Iterations = 1.0
    var size = matrix.size
    while (size > 2) {
        Iterations *= size
        size -= 1
    }
    return Iterations
}

fun searchDet(matrix: Array<Array<Double>>, Iterations: Double):Double {
    var det = 0.0
    var degree = 1
    if (matrix.size == 1) {
        print("\r>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|<<$100%")
        return matrix[0][0]
    } else if (matrix.size == 2) {
        var percent = ((indicator / Iterations) * 100.0).toInt()
        if ((((indicator - 1) / Iterations) * 100.0).toInt() < percent) {
            if (percent in 1..9) {
                print("\r>>${percent}%                          |<<")
            }
            if (percent in 10..19) {
                print("\r>>>>>${percent}%                      |<<")
            }
            if (percent in 20..29) {
                print("\r>>>>>>>>${percent}%                   |<<")
            }
            if (percent in 30..39) {
                print("\r>>>>>>>>>>${percent}%                 |<<")
            }
            if (percent in 40..49) {
                print("\r>>>>>>>>>>>>>${percent}%              |<<")
            }
            if (percent in 50..59) {
                print("\r>>>>>>>>>>>>>>>${percent}%            |<<")
            }
            if (percent in 60..69) {
                print("\r>>>>>>>>>>>>>>>>>${percent}%          |<<")
            }
            if (percent in 70..79) {
                print("\r>>>>>>>>>>>>>>>>>>>>${percent}%       |<<")
            }
            if (percent in 80..89) {
                print("\r>>>>>>>>>>>>>>>>>>>>>>>${percent}%    |<<")
            }
            if (percent in 90..99) {
                print("\r>>>>>>>>>>>>>>>>>>>>>>>>>>>${percent}%|<<")
            }
            if (percent == 100) {
                print("\r>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|<<${percent}%")
            }
        }
        indicator += 1.0
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]
    } else {
        val newMatrix: Array<Array<Double>> = Array(matrix.size - 1, { Array(matrix.size - 1, { 0.0 }) })
        for (j in 0 until matrix.size) {
            delColRow(matrix, 0, j, newMatrix)
            det += (degree * matrix[0][j] * searchDet(newMatrix, Iterations))
            degree = -degree
        }
    }
    return det
}





