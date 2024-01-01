echo "Compiling exit.."
cc exit.c -o exit
echo "Compiling CreateFlyer.."
cc CreateFlyer.c -o CreateFlyer
echo "Compiling MakeOrder.."
cc MakeOrder.c -o MakeOrder

echo "Compiling GetFlyer.."
cc GetFlyer.c -o GetFlyer
echo "Compiling GetNumComp.."
cc GetNumComp.c -o GetNumComp
echo "Compiling GetOrderNum.."
cc GetOrderNum.c -o GetOrderNum
echo "Compiling GetSummary.."
cc GetSummary.c -o GetSummary

echo "Starting the Advanced shell.."
cc Advanced_shell.c -o Advanced_shell
echo "Advanced shell compiled succesfully!"
echo ">>Starting.."
./Advanced_shell
echo "Advanced shell ended!<<"
