#ifndef UNTITLED2_MYNEURO_H
#define UNTITLED2_MYNEURO_H

///////////////// includes
#include <vector>
#include <fstream>
#include <random>
///////////////// pre-defining
using namespace std;
///////////////// templates
template<typename typC> ostream &operator<<(ostream &out,vector<typC> &a) { for (auto &x:a) out<<x<<" "; return out; }
///////////////// define some need-able classes
class Neuron;
class Network;
///////////////// some constants
const int MATRIX_SIZE=6;


class Neuron {
private:
    int value;

public:
    Neuron() {this->value=0;}
    Neuron(int value);

    void setValue(int val) {
        this->value=val;
    }

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
    // constants
    static const int DEPTH=5; // count of neuron square arrays
    const int ARR_SIZE[5]={6, 5, 4, 3, 2}; // sizes for neuron square array
    static const int RESULT_MAX_NUMBER=10; // maximum result for network
    const double EPS=1000; // delta for axons value
    static const int SIZE=256; // all count of axons values
    static const int NEURON_VISION_SIDE = 2; // one side of square of visible children neurons for parent
    static const int NEURON_VISION_COUNT=NEURON_VISION_SIDE*NEURON_VISION_SIDE; // count of axons for each neuron
    // arrays
    vector<vector<vector<Neuron>>> allNeurons; // main neurons
    vector<Neuron> resultNeurons; // neurons that identify numbers
    vector<double> axonsValues; // axonsValues for each axon of neuron
    vector<vector<string>> matrices; // all matrices for test the network
    vector<int> answer; // correct answers for each matrix
    // counter
    int LOG_COUNTER=0; // console log counter

public:
////////////    start-work functions
    Network() {
        allNeurons.resize(DEPTH);
        resultNeurons.resize(RESULT_MAX_NUMBER);
        for (int i = 0; i < DEPTH; ++i)
            allNeurons[i].resize(ARR_SIZE[i], vector<Neuron>(ARR_SIZE[i], Neuron()));
    }

////////////    main functions
//    validate one table function [ returns array of numbers that are similar to this one]
    bool check(vector<string> &value, int correct_answer) {
        // setting zero-level values to Neurons
        for (int i = 0; i < value.size(); ++i)
            for (int j = 0; j < value[i].size(); ++j)
                allNeurons[0][i][j].setValue(value[i][j] == '0' ? 0 : 1);

        // setting from first to DEPTH-1 level values to Neurons (that ones who are not making the answer)
        int last=0;
        for (int i = 1; i < DEPTH; ++i)
            for (int x = 0; x < allNeurons[i].size(); ++x) {
                for (int y = 0; y < allNeurons[i][x].size(); ++y) {
                    vector<double> axons_values(NEURON_VISION_COUNT);
                    vector<int> children_values;
                    for (auto &val : axons_values)
                        val = axonsValues[last++];
                    for (int child_x = 0; child_x < NEURON_VISION_SIDE; ++child_x)
                        for (int child_y = 0; child_y < NEURON_VISION_SIDE; ++child_y)
                            children_values.push_back(allNeurons[i-1][x+child_x][y+child_y].getValue());
                    allNeurons[i][x][y].updateValue(axons_values, children_values);
                }
            }

        // setting answers-Neurons values
        vector<int> children_values;
        for (int child_x = 0; child_x < NEURON_VISION_SIDE; ++child_x)
            for (int child_y = 0; child_y < NEURON_VISION_SIDE; ++child_y)
                children_values.push_back(allNeurons.back()[child_x][child_y].getValue());
        for (auto & resultNeuron : resultNeurons) {
            vector<double> axons_values(NEURON_VISION_COUNT);
            for (auto &val : axons_values)
                val = axonsValues[last++];
            resultNeuron.updateValue(axons_values, children_values);
        }

        // getting result
        vector<int> res;
        for (int i = 0; i < resultNeurons.size(); ++i)
            if (resultNeurons[i].getValue()==1)
                res.push_back(i);
        return (res.empty() && correct_answer==-1) || (res.size()==1 && correct_answer==res.back());
    }
//    check all matrices function
    bool check_all() {
        for (int i = 0; i < matrices.size(); ++i)
            if (!check(matrices[i], answer[i]))
                return false;
        return true;
    }
//    setting new axonsValues for axons function
    bool set_new_indexes() {
        while (!check_all()) {
            int index= random_int_number(SIZE);
            double new_value= random_double_number(-1, 1);
            if (LOG_COUNTER++==100)
                cout<<"trying "<<index<<" value: "<<new_value<<endl, LOG_COUNTER=0;
            axonsValues[index]=new_value;
            printAxonValues();
        }
        return true;
    }
//    function making a new setting cycle
    void make_new_indexes() {
        cout<<"Learning is in progress..."<<endl;
        bool result= set_new_indexes();
        if (result) {
            printAxonValues();
            cout<<"Learning is done"<<endl;
        }
        else cout<<"wrong network work"<<endl;
    }
//    getting result for matrix
    void get_result(vector<string> &value) {
        axonsValues.assign(SIZE, 0);
        readAxonValues();

        check(value, -1);
        vector<int> res;
        for (int i = 0; i < resultNeurons.size(); ++i)
            if (resultNeurons[i].getValue()==1)
                res.push_back(i);
        if (res.empty())
            cout<<"Your picture similar to none";
        else
            cout<<"Your picture similar to number: "<<res<<endl;

        printAxonValues();
    }
//    learn new matrix to network with define result
    void update(vector<string> &new_value, int result) {
        axonsValues.assign(SIZE, 0);
        matrices.assign(1, vector<string>(6));
        answer.assign(1,0);
        readMatrices();
        readAxonValues();

        if (!check(new_value, result)) {
            matrices.push_back(new_value);
            answer.push_back(result);
            make_new_indexes();
        }

        printMatrices();

        cout<<"upgrade done"<<endl;
        get_result(new_value);
    }

////////////    addons function
    double string_to_double(string &s) {
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
    int string_to_int(string &s) {
        int result=0;
        for (char i : s)
            result=result*10+(i-'0');
        return result;
    }
    int random_int_number(int mx) {
        return rand()%mx;
    }
    double random_double_number(double mn, double mx) {
        double f=((double)rand()/RAND_MAX);
        double result=mn+f*(mx-mn);
        result=round(result*EPS)/EPS;
        return result;
    }

////////////    write-read file functions
    void readAxonValues() {
        ifstream in("/home/boriskiva/CLionProjects/NeuralNetwork/network1/a_array.txt");

        for (double & i : axonsValues) {
            string s;
            getline(in, s);
            i = string_to_double(s);
        }

        in.clear();
        in.close();
    }
    void readMatrices() {
        ifstream in("/home/boriskiva/CLionProjects/NeuralNetwork/network1/previous_tables.txt");

        int i = 0, j = 0;
        if (in.is_open())
            while (in >> matrices[i][j++])
                if (j == MATRIX_SIZE) {
                    string s;
                    in>>s;
                    answer[i] = string_to_int(s);
                    matrices.emplace_back(MATRIX_SIZE);
                    answer.emplace_back();
                    i++, j = 0;
                }
        answer.pop_back();
        matrices.pop_back();

        in.close();
    }
    void printAxonValues() {
        ofstream out("/home/boriskiva/CLionProjects/NeuralNetwork/network1/a_array.txt");

        for (double & i : axonsValues)
            out<<i<<"\n";

        out.flush();
        out.close();
    }
    void printMatrices() {
        ofstream out("/home/boriskiva/CLionProjects/NeuralNetwork/network1/previous_tables.txt");

        for (int i = 0; i < matrices.size(); ++i) {
            for (const auto & j : matrices[i])
                out<<j<<"\n";
            out<<answer[i]<<'\n'<<"\n";
        }

        out.close();
    }
};

#endif
