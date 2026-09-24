import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import java.util.Scanner;

class Calculate {
    static double Calc(double x, int k) {
        double e = Math.pow(10.0, -k);
        double sum = 0;
        double temp = x;
        int n = 1;

        while (Math.abs(temp) >= e) {
            sum += temp;
            temp = temp * (-x) * n / (n + 1);
            n++;
        }
        return sum;
    }
}

class Calculate_BigDecimal {
    static BigDecimal CalcBig(BigDecimal X, int k) {
        MathContext mc = new MathContext(k + 10, RoundingMode.HALF_UP);
        BigDecimal e = BigDecimal.ONE.movePointLeft(k);

        BigDecimal sum = BigDecimal.ZERO;
        BigDecimal temp = X;

        BigDecimal bigN = BigDecimal.ONE;
        BigDecimal bigNPlus1 = BigDecimal.valueOf(2);
        BigDecimal One = BigDecimal.ONE;

        while (temp.abs().compareTo(e) >= 0) {
            sum = sum.add(temp, mc);

            temp = temp.multiply(X.negate(), mc)
                    .multiply(bigN, mc)
                    .divide(bigNPlus1, mc);

            bigN = bigN.add(One, mc);
            bigNPlus1 = bigNPlus1.add(One, mc);
        }

        return sum.setScale(k, RoundingMode.HALF_UP);
    }
}

public class TestOne {
    public static void main(String[] args) throws IOException {
        BufferedReader Read = new BufferedReader(new InputStreamReader(System.in));
        Scanner scanner = new Scanner(Read);

        System.out.print("Введите x (Double) (-1 < x <= 1): ");
        double x = scanner.nextDouble();

        System.out.print("Введите x (BigDecimal) (-1 < x <= 1): ");
        String s2 = Read.readLine();
        BigDecimal X = new BigDecimal(s2);

        System.out.print("Введите k (натуральное число): ");
        int k = scanner.nextInt();

        if (x <= -1 || x > 1 || k <= 0) {
            System.out.println("Ошибка: не соблюдено условие (-1 < x <= 1) или k <= 0!");
            return;
        }

        double answer_Math = Math.log(1 + x);
        double answer = Calculate.Calc(x, k);
        BigDecimal answer_BigDecimal = Calculate_BigDecimal.CalcBig(X, k);

        System.out.println("\n--- Результаты вычислений ---");
        System.out.printf("Math.log(1 + x):        %+." + (k + 1) + "f%n", answer_Math);
        System.out.printf("Сумма ряда (double):    %+." + (k + 1) + "f%n", answer);
        System.out.printf("Сумма ряда (BigDecimal):+%s%n", answer_BigDecimal);
    }
}
