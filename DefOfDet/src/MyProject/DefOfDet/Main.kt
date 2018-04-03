package MyProject.DefOfDet

fun main(args: Array<String>) {
    var count = 0.0
    var index = 0
    print("Введите матрицу в формате <a11 a12;a21 a22>: ")
    val str: String? = readLine()
    if (str == null) return
    val ptr_str = str.split(" |;".toRegex())
    for (c in ptr_str) {
        count += 1
    }
    count = Math.sqrt(count)
    val countI = count.toInt()
    val matrix: Array<Array<Double>> = Array(countI, { Array(countI, { 0.0 }) })
    for (i in 0 until countI) {
        for (j in 0 until countI) {
            matrix[i][j] = (ptr_str[index]).toDouble()
            index += 1
        }
    }
    print("Определитель матрицы равен: ")
    print(searchDet(matrix, countI))

}

fun delColRow(matrix: Array<Array<Double>>, countI: Int, rowForDel: Int, colForDel: Int, newMatrix: Array<Array<Double>>){
    var delRow = 0
    for(i in 0 until countI-1){
        if(i == rowForDel){
            delRow = 1
        }
        var delCol = 0
        for(j in 0..countI-2){
            if(j == colForDel){
                delCol = 1
            }
            newMatrix[i][j] = matrix[i + delRow][j + delCol]
        }
    }
}

fun searchDet(matrix: Array<Array<Double>>, countI: Int):Double{
    var det = 0.0
    var degree = 1
    if (countI == 1){
        return matrix[0][0]
    }
    else if (countI == 2){
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0]
    }
    else{
        val newMatrix: Array<Array<Double>> = Array(countI-1, { Array(countI-1, { 0.0 }) })
        for(j in 0 until countI) {
            delColRow(matrix, countI, 0, j, newMatrix)
            det += (degree * matrix[0][j] * searchDet(newMatrix, countI - 1))
            degree = -degree
        }
    }
    return det
}




