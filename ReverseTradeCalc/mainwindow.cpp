#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <stdio.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  bruteForce();
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

  //emit updateLog(workID, "\t Buy Amount: \t" + QString::number(*buyAmount) + "\t BTC");
  //emit updateLog(workID, "\t Buy Price: \t" + QString::number(*buyPrice) + "\t USD");
  //emit updateLog(workID, "\t Buy Total: \t" + QString::number(*buyTotal) + "\t USD");
}

void MainWindow::bruteForce() {
    double sellPrice = 2.5;
    double fee = 0.2;
    double sellAmount;
    double profit;

    double buyPrice = 0.0;
    double buyAmount;
    double buyTotal;
    double priceDifference = std::numeric_limits<double>::max();

    double bestSellAmount;
    double bestProfit;
    double bestBuyPrice;
    double bestBuyAmount;
    double bestBuyTotal;

    double maxFunds = 10;
    double oneSatoshi = 0.00001000;

    for(sellAmount = oneSatoshi; sellAmount < maxFunds; sellAmount += oneSatoshi) {
        for(profit = oneSatoshi; profit < maxFunds; profit += oneSatoshi) {
            calculateMinimumBuyTrade(sellPrice, sellAmount, fee, &buyPrice, &buyAmount, &buyTotal, profit);

            if((sellPrice - buyPrice) < priceDifference) {

                bestBuyPrice  = buyPrice;
                bestBuyAmount = buyAmount;
                bestBuyTotal  = buyTotal;

                bestSellAmount = sellAmount;
                bestProfit     = profit;

                priceDifference = (sellPrice - buyPrice);
            }
        }
    }

    std::cout << "Best Price: " << bestBuyPrice << " Best SellAmount: " << bestSellAmount << " Best Profit: " << bestProfit << std::endl;
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


buyAmount = ((sellAmount + profit) / (100.0 - fee)) * 100.0

sellAmount =



*/
