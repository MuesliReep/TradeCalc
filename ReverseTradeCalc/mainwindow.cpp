#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit) {

  // First calculate sell netto
  double sellTotal = sellAmount * sellPrice;
  double sellFee   = sellTotal  * (fee / 100.0);
  double sellNetto = sellTotal - sellFee;

  // Buy netto + profit = total - fee
  // So if fee is 0.2%, buy netto + profit = 99.8%
  // To find find 100% and thus the fee we divide by 99.8 and multiply by 100
  double buyNetto  = sellAmount + profit;
  *buyAmount = (buyNetto / (100.0 - fee)) * 100.0;

  // Last step is to find the buy price
  // buy price = buyTotal / buyAmount
  // buyTotal = sellNetto
  *buyTotal = sellNetto;
  *buyPrice = *buyTotal / *buyAmount;

  emit updateLog(workID, "\t Buy Amount: \t" + QString::number(*buyAmount) + "\t BTC");
  emit updateLog(workID, "\t Buy Price: \t" + QString::number(*buyPrice) + "\t USD");
  emit updateLog(workID, "\t Buy Total: \t" + QString::number(*buyTotal) + "\t USD");
}

// abs(sellprice - buyprice) = difference
/*

variables:
sellAmount : ! : Needs to be calculated
sellPrice  :   : Set by user
sellTotal  : ! : Needs to be calcluated

buyAmount  : ! : Needs to be calculated
buyPrice   :   : Determined by: sellprice - buySellDifference = buyPrice
buyTotal   : ! : Needs to be calculated

fee        :   : Set by user
sellFee    : ! : Needs to be calculated
buyFee     : ! : Needs to be calculated

buySellDifference : Set by user

sellTotal = (((sellPrice * sellAmount) * (fee / 100))
buyTotal  = (sellTotal * buyPrice)
profit    = (buyTotal * (100 - fee) / 100) - sellTotal

difference = abs(sellPrice - buyPrice)
difference = abs((sellTotal / sellAmount) - (buyTotal / buyAmount))
difference = abs((sellTotal / sellAmount) - ((sellTotal - sellFee) / (buyNetto / (100.0 - fee)) * 100.0)))
difference = abs((sellTotal / sellAmount) - ((sellTotal - sellFee) / ((sellAmount + profit) / (100.0 - fee)) * 100.0)))
difference = abs((sellTotal / sellAmount) - ((sellTotal - (sellTotal  * (fee / 100.0))) / ((sellAmount + profit) / (100.0 - fee)) * 100.0)))

Known variables:
difference = abs((sellTotal / sellAmount) - ((sellTotal - (sellTotal  * (fee / 100.0))) / ((sellAmount + profit) / (100.0 - fee)) * 100.0)))
   YES               NO          NO              NO           NO         YES                   NO          NO               YES

buyAmount = buyTotal / buyPrice
buyAmount = buyTotal / (sellPrice - difference)
buyAmount = (sellTotal - sellFee) / (sellPrice - difference)


*/
