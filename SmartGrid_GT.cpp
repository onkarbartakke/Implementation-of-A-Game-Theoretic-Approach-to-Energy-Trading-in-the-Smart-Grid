#include <bits/stdc++.h>

#define Ti 0.5
#define W 0.3
#define sigma 0.02

using namespace std;

class Seller
{
public:
    double supply;
    double reservationPrice;
    double expectedPrice;
    double previousSupply;

    Seller()
    {
        supply = 0.0;
        reservationPrice = 0.0;
        expectedPrice = 0.0;
    }
};

class Buyer
{
public:
    double demand;
    double reservationPrice;

    Buyer()
    {
        demand = 0.0;
        reservationPrice = 0.0;
    }
};

int GenerateRandom(int start, int end)
{
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
}

bool cmp1(Seller s1, Seller s2)
{
    if (s1.reservationPrice <= s2.reservationPrice)
        return true;
    else
        return false;
}

bool cmp2(Buyer b1, Buyer b2)
{
    if (b1.reservationPrice >= b2.reservationPrice)
        return true;
    else
        return false;
}

void Find_Intersection(int N, vector<Seller> &seller, vector<Buyer> &buyer, int selectedPersons[])
{
    double vol_seller = 0, vol_buyer = 0;
    bool found = false;

    int i, j;

    i = j = 0;

    vol_seller = seller[0].supply;
    vol_buyer = buyer[0].demand;

    if (seller[N - 1].reservationPrice > buyer[N - 1].reservationPrice)
    {
        while (!found && i < N - 1 && j < N - 1)
        {
            if (seller[i].reservationPrice <= buyer[j].reservationPrice && i < N - 1 && j < N - 1)
            {
                if (vol_seller < vol_buyer)
                {
                    vol_seller += seller[++i].supply;
                }
                else
                {
                    vol_buyer += buyer[++j].demand;
                }
            }
            else
                found = true;
        }

        selectedPersons[0] = i - 1;
        selectedPersons[1] = j - 1;
    }
    else
    {
        cout << "\n---------------- Seller Reservation is Lower than Buyer Reservation ------------------\n";
        selectedPersons[0] = -1;
        selectedPersons[1] = -1;
    }
}

double Best_Response_Strategy(double UnitePrice, int seller_i, int selectedPersons[], vector<Seller> &seller, vector<Buyer> &buyer)
{
    int BR;

    double supply = 0, demand = 0;

    for (int i = 0; i <= selectedPersons[0]; i++)
    {
        supply += seller[i].supply;
    }

    for (int i = 0; i <= selectedPersons[1]; i++)
    {
        demand += buyer[i].demand;
    }

    if (supply <= demand)
    {
        return ((UnitePrice - seller[seller_i].reservationPrice) / (2 * Ti));
    }
    else
    {
        return ((UnitePrice - seller[seller_i].reservationPrice) * selectedPersons[0] + 2 * Ti * ((supply - seller[seller_i].supply) - demand)) / ((2 * Ti) * (selectedPersons[0] - 2));
    }
}

bool Nash_Equlibirium(int selectedPersons[], vector<Seller> &seller, vector<Buyer> &buyer)
{
    int count = 1;

    for (int i = 0; i <= selectedPersons[0]; i++)
    {
        if (abs(seller[i].supply - seller[i].previousSupply) <= sigma)
            count++;
    }

    if (count == selectedPersons[0])
        return true;
    else
        return false;
}

int main()
{
    srand(time(0));

    int N = 10;

    vector<Seller> seller(N);
    vector<Buyer> buyer(N);

    for (int i = 0; i < N; i++)
    {
        buyer[i].reservationPrice = (double)GenerateRandom(15, 60);
        buyer[i].demand = (double)GenerateRandom(20, 60);

        seller[i].reservationPrice = (double)GenerateRandom(10, 50);
        seller[i].expectedPrice = (double)GenerateRandom(10, 50);
        seller[i].supply = (double)GenerateRandom(75, 220);
    }

    double array1[2][N];
    double array2[2][N];

    double bestResponses[N];

    int selectedPersons[2];

    // double previousSellerSupply[N];
    // vector<double>

    int number = 1;
    double UP;
    int cnt = 1;
    do
    {
        cout << "\n ---------- Iteration " << cnt << " started ---------- \n";

        /* code */
        sort(seller.begin(), seller.end(), cmp1);

        sort(buyer.begin(), buyer.end(), cmp2);

        bool found = false;

        // if (number > 1)
        // {
        //     for (int i = 0; i < N; i++)
        //     {
        //         seller[i].reservationPrice = (double)GenerateRandom(10, 50);
        //         seller[i].supply = (double)GenerateRandom(75, 220);
        //     }

        //     sort(seller.begin() , seller.end() , cmp1);
        // }

        while (!found)
        {
            Find_Intersection(N, seller, buyer, selectedPersons);
            cout << "\nSelected persons are " << selectedPersons[0] << " and " << selectedPersons[1] << "\n";

            if (selectedPersons[0] < 3 || selectedPersons[1] < 3)
            {
                for (int i = 0; i < N; i++)
                {
                    seller[i].reservationPrice = (double)GenerateRandom(10, 50);
                    seller[i].supply = (double)GenerateRandom(75, 220);
                }

                sort(seller.begin(), seller.end(), cmp1);
            }
            else
                found = true;
        }

        double unitPrice = (seller[selectedPersons[0]].reservationPrice + buyer[selectedPersons[1]].reservationPrice) / 2;
        UP = unitPrice;
        cout << "\n ******** Unit price for current Iteration is " << unitPrice << "*********\n";

        for (int i = 0; i <= selectedPersons[0]; i++)
        {
            seller[i].previousSupply = seller[i].supply;
        }

        for (int i = 0; i <= selectedPersons[0]; i++)
        {
            bestResponses[i] = Best_Response_Strategy(unitPrice, i, selectedPersons, seller, buyer);
        }

        double sup = 0, dem = 0;

        for (int i = 0; i <= selectedPersons[0]; i++)
            sup += seller[i].supply;

        for (int i = 0; i <= selectedPersons[1]; i++)
        {
            dem += buyer[i].demand;
        }

        cout << "\nNet Demand : " << dem << "\n";
        cout << "\nNet Supply : " << sup << "\n";

        for (int i = 0; i <= selectedPersons[0]; i++)
        {
            double randNum = ((double)GenerateRandom(0, 100)) / 100.0;

            if (randNum > 0.3)
            {
                seller[i].supply = bestResponses[i];
            }
            else
            {
                seller[i].supply = W * seller[i].supply;
            }
        }

        cout << "\n ---------- Iteration " << cnt << " completed ---------- \n";
        cnt++;
        number++;
    } while (!Nash_Equlibirium(selectedPersons, seller, buyer));


    for(int i = 0 ; i <= selectedPersons[0] ; i++)
    {
        cout<<"\nSeller "<<i<<" supply : "<<seller[i].supply<<" |  Reservation price : "<<seller[i].reservationPrice<<"\n";
    }

    cout<<"\n\n**********************\n\n";
    for(int i = 0 ; i <= selectedPersons[1] ; i++)
    {
        cout<<"\nBuyer "<<i<<" demand : "<<buyer[i].demand<<" |  Reservation price : "<<buyer[i].reservationPrice<<"\n";
    }
    cout << "\n---------- The Program is Over ---------\n";
    cout << "\n\n Final Seller supplies are : \n";

    for (int i = 0; i <= selectedPersons[0]; i++)
    {
        cout << seller[i].supply << "  ";
    }

    cout << "\n\nFinal Unite Price is : " << UP << "\n";
}