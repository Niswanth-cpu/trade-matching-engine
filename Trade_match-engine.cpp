// You Do NOT Suppose to Change the Starter Template except the Mentioned Parts.
// But if you do then I'm not responsible for your failed testcases.
// It is a Very Easy to do Lab Assignment. Best of Luck...

#include <bits/stdc++.h>
#include<fstream>
using namespace std;

struct Order {
    string user;
    int qty;
    double price;
};

class Book {
public:
    string ticker;
    vector<Order> buy, sell;
    Book(string t) : ticker(t) {}
};

bool compartor(Book& a,Book& b){
    if(a.ticker==b.ticker){
        return (a.buy.size()+a.sell.size())<(b.buy.size()+b.sell.size());
    }
    return a.ticker<b.ticker;
}

bool compartor2(Order& a,Order& b){
    return a.price < b.price;
}

bool compartor3(Order& a,Order& b){
    return a.price > b.price;
}

void part1() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    ifstream infile(file);
    //infile.open(file);
    if(!infile.is_open()){
        cout<<"NO file"<<endl;
    }
    stringstream ss;
    string line;
    vector<Book> companies;
    while(getline(infile,line)){

        if(line.empty()){
            break;
        }
        stringstream ss(line);
        string type,name,company,Quantity_str,price_str;
        getline(ss,type,',');
        getline(ss,name,',');
        getline(ss,company,',');
        getline(ss,Quantity_str,',');
        getline(ss,price_str);
        

        int idx=-1;
        for(int i=0;i<companies.size();i++){
            if (companies[i].ticker==company){
                idx=i;
                break;
            }
        }

        Order o;
        o.user=name;
        int quantity=stoi(Quantity_str);
        double price=stod(price_str);
        Book b(company);
        if(type=="BUY"){
            (b.buy).push_back(o);
        }
        else if(type=="SELL"){
            (b.sell).push_back(o);
        }
        companies.push_back(b);
    }
    sort(companies.begin(),companies.end(),compartor);
    map<string,int> list;
    
    for(int i=0;i<companies.size();i++){
        list[companies[i].ticker]++;
    }
    for(auto v : list){
        cout<<v.first<<" "<<v.second<<endl;
    }

}

void part2() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    ifstream infile(file);
    //infile.open(file);
    if(!infile.is_open()){
        cout<<"NO file"<<endl;
    }
    stringstream ss;
    string line;
    vector<Book> companies;
    vector<Order> v;
    while(getline(infile,line)){

        if(line.empty()){
            break;
        }
        stringstream ss(line);
        string type,name,company,Quantity_str,price_str;
        getline(ss,type,',');
        getline(ss,name,',');
        getline(ss,company,',');
        getline(ss,Quantity_str,',');
        getline(ss,price_str);
        
        
        int idx=-1;
        for(int i=0;i<companies.size();i++){
            if (companies[i].ticker==company)
            {
                idx=i;
                break;
            }
        }
        Order o;
        o.user=name;

        int quantity=stoi(Quantity_str);
        o.qty=quantity;
        v.push_back(o);
    }
    sort(companies.begin(),companies.end(),compartor);
    string s;
    
    while(cin>>s){
        if(s.empty()){
            break;
        }
        int Quantity=0;
        for(int i=0;i<v.size();i++){
            if(v[i].user==s){
                Quantity+=v[i].qty;
            }
        }
        cout<<s<<" "<<Quantity<<endl;
    }
    
}

void part3() {
    string filePath = "./actual_output/Q1/CSV/";
    string YOUR_ROLL_NUMBER = "CS24B044"; // Change with Your Actual Roll Number
    string fileName;
    cin >> fileName;
    string file = filePath + YOUR_ROLL_NUMBER + "/" + fileName;
    
    
    cin.ignore(1000, '\n');
    ofstream out;
    out.open(file);
    if(!out.is_open()){
        cout<<"No file found"<<endl;
    }
    out<<"Ticker,Seller,Buyer,Qty,Price,Time\n";
    stringstream ss;
    string line;
    
    vector<Book> v;
    int time=0; 
    while(getline(cin,line)){
        if(line.empty()) break;

        stringstream ss(line);
        string type,name,company,Quantity_str,price_str;

        getline(ss,type,' ');
        getline(ss,name,' ');
        getline(ss,company,' ');
        getline(ss,Quantity_str,' ');
        getline(ss,price_str);
        
        Order o;
        o.price=stod(price_str);
        o.qty=stoi(Quantity_str);
        o.user=name;

        int idx=-1;
        for(int i=0;i<v.size();i++){
            if(v[i].ticker==company){
                idx=i;
                break;
            }
        }

        if(idx==-1){
            Book b(company);
            if(type=="SELL"){
                b.sell.push_back(o);
                v.push_back(b);
            }
            else if(type=="BUY"){
                b.buy.push_back(o);
                v.push_back(b);
            }
            
        }
        else{
            
            if(type=="SELL")
            {
                sort(v[idx].buy.begin(), v[idx].buy.end(), compartor3);
                while(o.qty>0 && !v[idx].buy.empty())
                {
                    if(v[idx].buy[0].price < o.price)
                        break;

                    int tradeQty=min(o.qty,v[idx].buy[0].qty);

                    out<<company<<","<<o.user<<","<<v[idx].buy[0].user<<","<<tradeQty<<","<<fixed<<setprecision(2)<<o.price<<","<<time++<<"\n";

                    o.qty-=tradeQty;
                    v[idx].buy[0].qty-=tradeQty;

                    if(v[idx].buy[0].qty==0)
                        v[idx].buy.erase(v[idx].buy.begin());
                }

                if(o.qty>0)
                    v[idx].sell.push_back(o);
            }
            else if(type=="BUY")
            {
                sort(v[idx].sell.begin(), v[idx].sell.end(), compartor2);

                while(o.qty>0 && !v[idx].sell.empty())
                {
                    if(v[idx].sell[0].price > o.price)
                        break;

                    int tradeQty=min(o.qty,v[idx].sell[0].qty);

                    out<<company<<","<<v[idx].sell[0].user<<","<<o.user<<","<<tradeQty<<","<<fixed<<setprecision(2)<<v[idx].sell[0].price<<","<<time++<<"\n";

                    o.qty-=tradeQty;
                    v[idx].sell[0].qty-=tradeQty;

                    if(v[idx].sell[0].qty==0)
                        v[idx].sell.erase(v[idx].sell.begin());
                }

                if(o.qty>0)
                    v[idx].buy.push_back(o);
            }
        }
    } 
    out.close();
}

int main() {
    string part;
    cin >> part;
    
    if (part == "P1") {
        part1();
    } else if (part == "P2") {
        part2();
    } else {
        part3();
    }
    return 0;
}
