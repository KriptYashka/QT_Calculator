#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Кучук Егор, калькулятор");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Для умножения и деления */
double digit_1 = 0;
double digit_2 = 0;

/* Для суммы и разности */
int digit1_main = 0; // число до запятой
int digit1_dot = 0; // число после запятой ( без ведущих нулей )
int digit1_dot_shift = 0; // свдиг числа после запятой или просто количество ведущих нулей

int digit2_main = 0;
int digit2_dot = 0;
int digit2_dot_shift = 0;

int select = 0; // выбор арифметического действия
bool isChanged = false; // флаг, обозначающий, был ли изменен арифметический знак на другой
/*
SELECT
0 - ничего
1 - сумма
2 - разность
3 - произведение
4 - деление
*/

void MainWindow::disable_btn(bool flag){
    /* Делает рабочими/нерабочими кнопки арифметических действий */
    ui->btn_plus->setDisabled(flag);
    ui->btn_minus->setDisabled(flag);
    ui->btn_multiply->setDisabled(flag);
    ui->btn_divide->setDisabled(flag);
}

void MainWindow::set_number(QString num){
    /* Добавляет новую цифру к числу */
    isChanged = false; // флаг, обозначающий, был ли изменен арифметический знак на другой
    QString text = ui->lineEdit->text();
    bool flag_dot = false; // есть ли точка в числе
    int after_dot = 0; // чисел после точки
    int before_dot = 0; // чисел до точки

    /* Несколько слагаемых */
    if (select != 0){
       disable_btn(true);
    } else {
       disable_btn(false);
    }

    for (int i = 0; i < text.length(); ++i){
        if (flag_dot){
            after_dot++;
        }
        if (!flag_dot && text[i].isDigit()){
            before_dot++;
        }
        if (text[i] == '.'){
            flag_dot = true;
        }
    }
    if (text == "0" || text == "Error"){
        ui->lineEdit->setText(num);
        return;
    }
    if (after_dot >= 9 || ((!flag_dot) && before_dot >= 8)){
        return;
    }
    ui->lineEdit->setText(text + num);
}

void MainWindow::setSign(QString sign){
    /* Устанавливает арифметическое действие над числами */
    if (!isChanged){
        get_digit(ui->lineEdit->text(), 1);
        digit_1 = ui->lineEdit->text().toDouble();
        ui->tips->setText(ui->lineEdit->text() + " " + sign + " ");
        ui->lineEdit->setText("0");
        isChanged = true;
    } else {
        QString str = ui->tips->text();
        str.resize(str.size() - 2);
        ui->tips->setText(str + sign + " ");
    }
}

void MainWindow::on_btn_0_clicked() {
    QString text = ui->lineEdit->text();
    if (text == "0"){
        return;
    }
    if (text == "Error")
        ui->lineEdit->setText("");
    //ui->lineEdit->setText(text + "0");
    set_number("0");
}

void MainWindow::on_btn_1_clicked() {
    set_number("1");
}

void MainWindow::on_btn_2_clicked() {
    set_number("2");
}

void MainWindow::on_btn_3_clicked() {
    set_number("3");
}

void MainWindow::on_btn_4_clicked() {
    set_number("4");
}

void MainWindow::on_btn_5_clicked() {
    set_number("5");
}

void MainWindow::on_btn_6_clicked() {
    set_number("6");
}

void MainWindow::on_btn_7_clicked() {
    set_number("7");
}

void MainWindow::on_btn_8_clicked() {
    set_number("8");
}

void MainWindow::on_btn_9_clicked() {
    set_number("9");
}

void MainWindow::on_btn_plus_clicked() {
    select = 1;
    setSign("+");
}

void MainWindow::on_btn_minus_clicked() {
    select = 2;
    setSign("-");
}

void MainWindow::on_btn_multiply_clicked() {
    select = 3;
    setSign("*");
}

void MainWindow::on_btn_divide_clicked() {
    select = 4;
    setSign("/");
}

void MainWindow::get_digit(QString str, int select_digit) {
    /* "Расфасовывает" число на: целую часть, дробную часть, свдиг дробной части от точки */
    int position_dot = str.indexOf(".");
    int *digit_main, *digit_dot, *digit_dot_shift;
    if (select_digit == 1){
        digit_main = &digit1_main;
        digit_dot = &digit1_dot;
        digit_dot_shift = &digit1_dot_shift;
    } else {
        digit_main = &digit2_main;
        digit_dot = &digit2_dot;
        digit_dot_shift = &digit2_dot_shift;
    }
    *digit_main = 0;
    *digit_dot = 0;
    *digit_dot_shift = 0;
    int d = 1;
    if (position_dot == -1){ // Для целых чисел
        position_dot = str.length();
    }
    for(int i = str.length() - 1; i > position_dot; --i){
        int current_digit = str[i].digitValue();
        *digit_dot += d * current_digit;
        d *= 10;
        if (current_digit == 0){
            (*digit_dot_shift)++;
        } else {
            *digit_dot_shift = 0;
        }
    }
    d = 1;
    for(int i = position_dot - 1; i >= 0; --i){
        if (str[i] == '-'){
            *digit_main *= -1;
            *digit_dot *= -1;
            continue;
        }
        int current_digit = str[i].digitValue();
        *digit_main += d * current_digit;
        d *= 10;
    }
}

int len_int(long long int a){
    /* Подсчет количества цифр */
    int k = 0;
    while (a != 0){
        k++;
        a /= 10;
    }
    return k;
}

int max(int a, int b){
    if (a > b)
        return a;
    return b;
}

int min(int a, int b){
    if (a < b)
        return a;
    return b;
}

QString MainWindow::sum_digit(int select = 0) {
    /* Суммирование двух чисел с высокой точностью */
    if (select == 1){
        digit2_main *= -1;
        digit2_dot *= -1;
    }
    int digit_main = digit1_main + digit2_main;
    int digit_dot, digit_dot_shift;

    int multi = pow(10, abs(digit1_dot_shift + len_int(digit1_dot) - digit2_dot_shift - len_int(digit2_dot)));

    /* То число, что меньше сдвинута, нужно домножить на multi */
    int len_before = max(len_int(digit1_dot), len_int(digit2_dot));
    int len_general = max(digit1_dot_shift + len_int(digit1_dot), digit2_dot_shift + len_int(digit2_dot));
    if (digit1_dot_shift + len_int(digit1_dot) > digit2_dot_shift + len_int(digit2_dot)){
        // Если общая длина digit1 больше
        digit_dot = digit1_dot + digit2_dot * multi;
    } else {
        // Если общая длина digit2 больше
        digit_dot = digit2_dot + digit1_dot * multi;
    }
    int len_after = len_int(digit_dot);
    digit_dot_shift = min(digit1_dot_shift, digit2_dot_shift) - abs(len_before - len_after);
    digit_dot_shift = len_general - len_after;

    if (digit_dot_shift < 0){
        digit_main += digit_dot / pow(10, -1 * len_before * digit_dot_shift);
        digit_dot = digit_dot % (int)pow(10, -1 * len_before * digit_dot_shift);
        digit_dot_shift = abs((-1 * len_before * digit_dot_shift) - len_int(digit_dot));
    }

    while (digit_dot % 10 == 0 && digit_dot != 0){ // удаление незначащих нулей
        digit_dot /= 10;
    }

    QString text = QString::number(digit_main);
    if  (digit_dot != 0){
        text += ".";
        for (int i = 0; i < digit_dot_shift; ++i){
            text += "0";
        }
        text += QString::number(abs(digit_dot));
    }
    return text;
}

QString rework_restext(QString restext){
    /* Удаление незначащих нулей в строке + удаление точки, если нет дробной части */
    if (restext.contains('.')){
        for (int i = restext.length() - 1; i > 0; --i){
            if (restext[i] == '0'){
                restext.resize(restext.length() - 1);
            } else if ((i == restext.length() - 1) && (restext[i] == '.')){
                restext.resize(restext.length() - 1);
                break;
            } else {
                break;
            }
        }
    }
    return restext;
}

double set_main_size(double res){
    /* Ограничивает целую часть до 8 символов */
    int count = 0;
    int c = (int)res;
    while (c != 0){
        c /= 10;
        count++;
    }
    if (count > 8)
        res /= pow(10, count - 8);
    return res;
}

void MainWindow::on_btn_equal_clicked(){
    /* Вычисление результата */
    if (select == 0){
        return;
    }
    get_digit(ui->lineEdit->text(), 2);
    digit_2 = ui->lineEdit->text().toDouble();
    double result = 0;
    QString sign = "";
    QString restext = "";
    bool overflow = false;

    disable_btn(false);

    if (select == 1){
        restext = sum_digit();
        sign = "+";
    }
    if (select == 2){
        restext = sum_digit(1);
        sign = "-";

    }
    if (select == 3){
        double result2 = digit_1 * digit_2;
        result = set_main_size(result2);
        if (result2 != result){ // Если результат выходит за гранницы допустимых значений ( до 8 знаков )
            overflow = true;
        }
        restext = QString::number(result, 'f', 9);
        sign = "*";
    }
    if (select == 4){
        sign = "/";
        if (digit2_main == 0 && digit2_dot == 0){ // проверка деления на нуль
            ui->lineEdit->setText("Error");
            ui->tips->setText("Деление на нуль");
            return;
        }
        double result2 = digit_1 / digit_2;
        result = set_main_size(result2);
        if (result2 != result){ // Если результат выходит за гранницы допустимых значений ( до 8 знаков )
            overflow = true;
        }
        restext = QString::number(result, 'f', 9);
    }
    /* Вывод результата */
    QString d1, d2;
    if ((sign == "+") || (sign == "-")){ // для точных чисел
        d1.setNum(digit1_main);
        if  (digit1_dot != 0){
            d1 += ".";
            for (int i = 0; i < digit1_dot_shift; ++i){
                d1 += "0";
            }
            d1 += QString::number(abs(digit1_dot));
        }
        d2.setNum(abs(digit2_main));
        if  (digit2_dot != 0){
            d2 += ".";
            for (int i = 0; i < digit2_dot_shift; ++i){
                d2 += "0";
            }
            d2 += QString::number(abs(digit2_dot));
        }
    } else { // для обычных чисел при умножении, делении
        d1 = QString::number(digit_1, 'f', 9);
        d2 = QString::number(digit_2, 'f', 9);
    }
    restext = rework_restext(restext);
    d1 = rework_restext(d1);
    d2 = rework_restext(d2);
    if (overflow) {
        ui->tips->setText(d1 + " " + sign + " " + d2 + "\nЧисло превысило лимит знаков.");
    } else {
        ui->tips->setText(d1 + " " + sign + " " + d2);
    }
    ui->lineEdit->setText(restext);
    select = 0;
}

void MainWindow::on_btn_dot_clicked()
{
    QString text = ui->lineEdit->text();
    if (text == "Error" || text == ""){
        ui->lineEdit->setText("0.");
        return;
    }
    if (!text.contains("."))
        ui->lineEdit->setText(text + ".");
}

void MainWindow::on_btn_clear_clicked()
{
    ui->lineEdit->setText("0");
    select = 0;
    digit_1 = 0;
    digit_2 = 0;
    ui->tips->setText("Очищено");
}

void MainWindow::on_btn_back_clicked()
{
    /* Стирание одного знака */
    QString text = ui->lineEdit->text();
    if (text == "0" || text == "Error" || text == "0." || text == "-0."){
        ui->lineEdit->setText("0");
        disable_btn(false);
        return;
    }
    text.resize(text.size() - 1);
    if (text == "" || text == "-"){
        text = "0";
        //isChanged = true;
        disable_btn(false);
    }
    ui->lineEdit->setText(text);

}

void MainWindow::on_btn_sign_clicked()
{
    /* Меняет знак числа */
    QString text = ui->lineEdit->text();
    if (text == "Error" || text == "0" || text == "0.")
        return;
    if (text.contains("-")){
        text.replace("-", "");
    } else {
        text = "-" + text;
    }
    ui->lineEdit->setText(text);
}
