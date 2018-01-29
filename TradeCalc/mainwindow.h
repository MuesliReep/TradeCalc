#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void calcSellAmountChanged();
  void calcBuyAmountChanged();
  void calcSellPriceChanged();
  void calcBuyPriceChanged();
  void calcSellTotalValueChanged();
  void calcBuyTotalValueChanged();
  void calcFeeValueChanged();
  void calcProfitValueChanged();

private:
  Ui::MainWindow *ui;
  void calcValueChanged(int value);
  void calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit = 0.00000001);
  double calculateMinimumSellTrade(double sellPrice, double sellAmount, double fee, double profit = 0.00000001);
  void calculateMinimumBuyTrade2(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit);
  void calculateMinimumBuyTrade3(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit);
  double calculateBitstampFee(double orderTotal);
};

#endif // MAINWINDOW_H
