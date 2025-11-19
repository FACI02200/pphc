package com.openpajak.pph

/**
 * Kotlin extensions for PPHC library
 */

// Extension functions for PPHMoney
operator fun PPHMoney.plus(other: PPHMoney): PPHMoney = this.add(other)
operator fun PPHMoney.minus(other: PPHMoney): PPHMoney = this.subtract(other)
operator fun PPHMoney.times(other: PPHMoney): PPHMoney = this.multiply(other)
operator fun PPHMoney.div(divisor: Long): PPHMoney = this.divide(divisor)
operator fun PPHMoney.compareTo(other: PPHMoney): Int = this.compareTo(other)

// Extension for creating money from Long
val Long.idr: PPHMoney get() = PPHMoney.fromRupiah(this)
val Int.idr: PPHMoney get() = PPHMoney.fromRupiah(this.toLong())

// Extension for parsing Indonesian format
fun String.toMoney(): PPHMoney = PPHMoney.parseIndonesian(this)

// DSL for PPh21 input
class PPh21InputBuilder(private val brutoMonthly: PPHMoney) {
    private val input = PPH21Calculator.PPH21Input(brutoMonthly)

    fun monthsPaid(months: Int) = apply { input.monthsPaid = months }
    fun ptkpStatus(status: PPHCalculator.PTKPStatus) = apply { input.ptkpStatus = status }
    fun scheme(scheme: PPH21Calculator.Scheme) = apply { input.scheme = scheme }
    fun terCategory(category: PPHCalculator.TERCategory) = apply { input.terCategory = category }
    fun pensionContribution(amount: PPHMoney) = apply { input.pensionContribution = amount }
    fun zakatOrDonation(amount: PPHMoney) = apply { input.zakatOrDonation = amount }

    fun bonus(month: Int, amount: PPHMoney, name: String) = apply {
        input.withBonus(month, amount, name)
    }

    fun build(): PPH21Calculator.PPH21Input = input
}

// DSL function
fun pph21Input(brutoMonthly: PPHMoney, block: PPh21InputBuilder.() -> Unit): PPH21Calculator.PPH21Input {
    return PPh21InputBuilder(brutoMonthly).apply(block).build()
}

// Extension function for calculation
fun PPH21Calculator.PPH21Input.calculate(): PPH21Calculator.PPH21Result {
    return PPH21Calculator.calculate(this)
}

// Usage examples (not compiled, just for documentation)
/*
fun exampleUsage() {
    // Initialize
    PPHCalculator.initialize()

    // Simple money operations with operator overloading
    val salary = 10_000_000.idr
    val bonus = 5_000_000.idr
    val total = salary + bonus

    // Parse Indonesian format
    val amount = "250.000.000,50".toMoney()

    // DSL for PPh21
    val result = pph21Input(250_000_000.idr) {
        monthsPaid(12)
        ptkpStatus(PPHCalculator.PTKPStatus.K2)
        scheme(PPH21Calculator.Scheme.TER)
        terCategory(PPHCalculator.TERCategory.B)
        bonus(3, 250_000_000.idr, "THR")
        bonus(4, 100_000_000.idr, "Bonus Tahunan")
    }.calculate()

    result.use {
        println("Total tax: ${it.totalTax}")
        it.breakdown.forEach { row ->
            println(row)
        }
    }

    // Or with auto-close
    pph21Input(10_000_000.idr) {
        monthsPaid(12)
        ptkpStatus(PPHCalculator.PTKPStatus.TK0)
        scheme(PPH21Calculator.Scheme.TER)
        terCategory(PPHCalculator.TERCategory.B)
    }.calculate().use { result ->
        // Use result
        val tax = result.totalTax
    }
}
*/
