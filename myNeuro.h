#ifndef UNTITLED2_MYNEURO_H
#define UNTITLED2_MYNEURO_H

#include <vector>
#include <fstream>
#include <random>

using namespace std;

class Neuron;
class Network;
struct Axon;

struct Axon {
public:
    double a;
    Neuron* child;

    Axon();
    Axon(Neuron *new_child) { child = new_child; }
};

class Neuron {
private:
    int value;

public:
    Neuron() {this->value=0;}
    Neuron(int value);

    void setValue(int val) { this->value=val; }
    void updateValue(vector<double> &val, vector<int> &child_val) {
        double new_value=0;
        for(int i=0; i<4; ++i)
            new_value+=child_val[i]*val[i];

        if (new_value>0) this->value=1;
        else this->value=0;
    }

    int getValue() {
        return value;
    }
};

class Network {
private:
    vector<vector<vector<Neuron>>> a;
    const double EPS=1e-3;
    const int SIZE=4*(25+16+9+4+1);
    vector<double> indexes;
    vector<vector<string>> matrices;
    vector<int> answer;

public:
//    start-work functions
    Network() {
        a.resize(6);
        for (int i = 0; i < 6; ++i)
            a[i].resize((6-i), vector<Neuron>(6-i));
        for (int i = 1; i < 6; ++i)
            for (auto & x : a[i])
                for (auto & y : x)
                    y = Neuron();
    }
//    children get function
    Neuron getNeuron(int d, int x, int y) {
        return a[d][x][y];
    }

//    main functions
    bool check(vector<string> &value) {
        for (int i = 0; i < value.size(); ++i)
            for (int j = 0; j < value[i].size(); ++j)
                a[0][i][j].setValue(value[i][j]=='0'?0:1);

        int last=0;
        for (int i = 1; i < 6; ++i)
            for (int x = 0; x < a[i].size(); ++x) {
                for (int y = 0; y < a[i][x].size(); ++y) {
                    vector<double> axons_values(4);
                    vector<int> children_values;
                    for (double & val : axons_values)
                        val=indexes[last++];
                    children_values.push_back(a[i-1][x][y].getValue());
                    children_values.push_back(a[i-1][x+1][y].getValue());
                    children_values.push_back(a[i-1][x][y+1].getValue());
                    children_values.push_back(a[i-1][x+1][y+1].getValue());
                    a[i][x][y].updateValue(axons_values, children_values);
                }
            }

        if (a[5][0][0].getValue() == 0)
            return false;
        return true;
    }
    bool checkAll() {
        for (int i = 0; i < matrices.size(); ++i) {
            bool result = answer[i]==1;
            if (check(matrices[i]) != result) return false;
        }
        return true;
    }
    bool set_new_indexes() {
        while (!checkAll()) {
            int index=random_int_fun(SIZE);
            double new_value=random_double_fun(-1, 1);
            cout<<"trying "<<index<<" value: "<<new_value<<"\n";
            indexes[index]=new_value;
            printA();
        }
        return true;
    }
    void make_new_indexes() {
        cout<<"recursion is working...\n";
        bool result= set_new_indexes();
        if (result) {
            printA();
            cout<<"recursion done\n";
        }
        else cout<<"wrong network work\n";
    }

    void get_result(vector<string> &value) {
        check(value);
        if (a[5][0][0].getValue()==0) cout<<"Not a seven\n";
        else cout<<"It is a seven\n";
    }
    void update(vector<string> &new_value, int result) {
        indexes.assign(SIZE,0);
        matrices.assign(1, vector<string>(6));
        answer.assign(1,0);
        readMatrices();
        readA();

        bool correct_result = result == 1;
        matrices.push_back(new_value);
        answer.push_back(result);
        if (check(new_value) != correct_result)
            make_new_indexes();

        printMatrices();

        cout<<"upgrade done\n";
        get_result(new_value);
    }

//    addons function
    double getDoubleNumber(string &s) {
        double res=0, power=10;
        int i=0;
        bool sign=false;
        if (s[i]=='-')
            sign=true,i++;
        while (s[i]!='.'&&i<s.size())
            res=res*10+(s[i++]-'0');
        i++;
        while(i<s.size())
            res=res+(s[i++]-'0')/power, power*=10;

        if (sign)
            return res*-1;
        return res;
    }
    int getIntNumber(string &s) {
        int result=0;
        for (char i : s)
            result=result*10+(i-'0');
        return result;
    }
    int random_int_fun(int mx) {
        return rand()%mx;
    }
    double random_double_fun(double mn, double mx) {
        double f=((double)rand()/RAND_MAX);
        double result=mn+f*(mx-mn);
        result=round(result*1000)/1000;
        return result;
    }

//    write-read file functions
    void readA() {
        ifstream in("/home/boriskiva/CLionProjects/NeuralNetwork/a_array.txt");

        for (double & i : indexes) {
            string s;
            getline(in, s);
            i = getDoubleNumber(s);
        }

        in.clear();
        in.close();
    }
    void readMatrices() {
        ifstream in("/home/boriskiva/CLionProjects/NeuralNetwork/previous_tables.txt");

        int i = 0, j = 0;
        if (in.is_open())
            while (in >> matrices[i][j++])
                if (j == 6) {
                    string s;
                    in>>s;
                    answer[i] = getIntNumber(s);
                    matrices.emplace_back(6);
                    answer.emplace_back();
                    i++, j = 0;
                }
        answer.pop_back();
        matrices.pop_back();

        in.close();
    }
    void printA() {
        ofstream out("/home/boriskiva/CLionProjects/NeuralNetwork/a_array.txt");

        for (double & i : indexes)
            out<<i<<"\n";

        out.flush();
        out.close();
    }
    void printMatrices() {
        ofstream out("/home/boriskiva/CLionProjects/NeuralNetwork/previous_tables.txt");

        for (int i = 0; i < matrices.size(); ++i) {
            for (const auto & j : matrices[i])
                out<<j<<"\n";
            out<<answer[i]<<'\n'<<"\n";
        }

        out.close();
    }

};

#endif
