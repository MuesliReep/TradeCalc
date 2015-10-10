#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);


  // Trade calc widget
  connect(ui->lineEditCalcSellAmount, SIGNAL(textEdited(const QString)),this,SLOT(calcSellAmountChanged()));
  connect(ui->lineEditCalcSellPrice,  SIGNAL(textEdited(const QString)),this,SLOT(calcSellPriceChanged()));
  connect(ui->lineEditCalcSellTotal,  SIGNAL(textEdited(const QString)),this,SLOT(calcSellTotalValueChanged()));
  connect(ui->lineEditCalcFee,        SIGNAL(textEdited(const QString)),this,SLOT(calcFeeValueChanged()));
  connect(ui->lineEditCalcBuyAmount,  SIGNAL(textEdited(const QString)),this,SLOT(calcBuyAmountChanged()));
  connect(ui->lineEditCalcBuyPrice,   SIGNAL(textEdited(const QString)),this,SLOT(calcBuyPriceChanged()));
  connect(ui->lineEditCalcBuyTotal,   SIGNAL(textEdited(const QString)),this,SLOT(calcBuyTotalValueChanged()));
  connect(ui->lineEditCalcProfit,     SIGNAL(textEdited(const QString)),this,SLOT(calcProfitValueChanged()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

//
void MainWindow::calcValueChanged(int value) {

  double sellAmount = ui->lineEditCalcSellAmount->text().toDouble();
  double sellPrice  = ui->lineEditCalcSellPrice->text().toDouble();
  double sellTotal  = ui->lineEditCalcSellTotal->text().toDouble();

  double buyAmount  = ui->lineEditCalcBuyAmount->text().toDouble();
  double buyPrice   = ui->lineEditCalcBuyPrice->text().toDouble();
  double buyTotal   = ui->lineEditCalcBuyTotal->text().toDouble();

  double fee = ui->lineEditCalcFee->text().toDouble();
  double sellFee  = 0.0;
  double buyFee   = 0.0;

  double profit = ui->lineEditCalcProfit->text().toDouble();

  switch(value) {

    case 0: // sell amount or price changed
      sellTotal = sellAmount * sellPrice;
    break;
    case 1: // buy amount or price changed
      buyTotal = buyAmount * buyPrice;
    break;
    case 2: // sell total changed
      sellAmount = sellTotal / sellPrice;
    break;
    case 3: // buy total changed
      buyAmount = buyTotal / buyPrice;
    break;
    case 4: // fee percentage changed
    break;
    case 5: // profit changed
    break;
  }

  if(value == 0 || value == 2 || value == 4 || value == 6) { // Calculate minimum buy amount
    calculateMinimumBuyTrade(sellPrice, sellAmount, fee, &buyPrice, &buyAmount, &buyTotal);
  }
  else if (value == 1 || value == 5 || value == 3) { // Calculate minimum sell amount
    // calculateMinimumSellTrade(buyPrice, buyAmount, fee);
  }
  else if (value == 7) {
    calculateMinimumBuyTrade(sellPrice, sellAmount, fee, &buyPrice, &buyAmount, &buyTotal, profit);
  }
  else{
    qDebug() << "bad value";
  }

  sellFee = sellTotal * (fee / 100);
  buyFee  = buyTotal  * (fee / 100);

  QString sSellAmount;
  QString sSellPrice;
  QString sSellTotal;
  QString sBuyAmount;
  QString sBuyPrice;
  QString sBuyTotal;
  QString sFee;
  QString sSellFee;
  QString sBuyFee;

  // Only update fields that were not changed by the user:
  if(value != 5)
    ui->lineEditCalcBuyTotal->setText(sSellAmount.setNum(buyTotal,'f',3));
  if(value != 3)
    ui->lineEditCalcBuyPrice->setText(sBuyPrice.setNum(buyPrice,'f',3));
  if(value != 1)
    ui->lineEditCalcBuyAmount->setText(sBuyAmount.setNum(buyAmount,'f',8));
  if(value != 6)
    ui->lineEditCalcFee->setText(sFee.setNum(fee,'f',8));
  if(value != 4)
    ui->lineEditCalcSellTotal->setText(sSellTotal.setNum(sellTotal,'f',3));
  if(value != 2)
    ui->lineEditCalcSellPrice->setText(sSellPrice.setNum(sellPrice,'f',3));
  if(value != 0)
    ui->lineEditCalcSellAmount->setText(sSellAmount.setNum(sellAmount,'f',8));
  ui->labelCalcFeeSellAmount->setText(sSellFee.setNum(sellFee,'f',8));
  ui->labelCalcFeeBuyAmount->setText(sBuyFee.setNum(buyFee,'f',8));

}

void MainWindow::calcSellAmountChanged()      { calcValueChanged(0); }
void MainWindow::calcBuyAmountChanged()       { calcValueChanged(1); }
void MainWindow::calcSellPriceChanged()       { calcValueChanged(2); }
void MainWindow::calcBuyPriceChanged()        { calcValueChanged(3); }
void MainWindow::calcSellTotalValueChanged()  { calcValueChanged(4); }
void MainWindow::calcBuyTotalValueChanged()   { calcValueChanged(5); }
void MainWindow::calcFeeValueChanged()        { calcValueChanged(6); }
void MainWindow::calcProfitValueChanged()     { calcValueChanged(7); }
//void MainWindow::calcUseButtonPressed()       { }

void MainWindow::calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit) {

  double x = 0.00000001;
  double i = 0;

  i = profit + sellAmount;
  i = i / (1.0-fee/100.0);
  i = ((sellPrice * sellAmount) * (1.0-fee/100.0)) / i;
  i = (sellPrice - i) / x;
  i++; // +1 to go over break even point

  profit = ((((sellPrice * sellAmount) * (1.0-fee/100.0)) / (sellPrice - i*x)) * (1.0-fee/100.0)) - sellAmount;

  qDebug() << "i= " << i << "\tprofit= " << profit;

  *buyAmount  = (((sellPrice * sellAmount) * (1.0-fee/100.0)) / (sellPrice - i*x));
  *buyPrice   = sellPrice - i * x;
  *buyTotal   = *buyAmount * *buyPrice;

  qDebug() << "\t Buy Amount: \t" << *buyAmount << "\t BTC";
  qDebug() << "\t Buy Price: \t" << *buyPrice << "\t USD";
  qDebug() << "\t Buy Total: \t" << *buyTotal << "\t USD";
}
