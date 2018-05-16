






#include <iostream>
#include <cstdio>
#include <string>
#include <random>
#include <math.h>
#include <vector>


using namespace std;
double tao = 0.01;
double TAO = 0.01;
double stepSizeBound = 0.000001;
random_device rd;
random_device rdArr[10];

class individual{
private:
    double x[10];
    double step[10];
    double stepSizeBound = 0.000001;
    
    double MutationCount = 0;
    double successfulMutationCount = 0;
    double oneFiveRuleAlpha = 0.9;
    default_random_engine generator;
    normal_distribution<double> distribution;
    default_random_engine gen[10];
    

public:
    void print(){
        for(int i=0; i<10; i++) printf("%lf ", x[i]);
        printf("\n");
        for(int i=0; i<10; i++) printf("%lf ", step[i]);
        printf("\n\n\n");
    }
    
    
    individual(double initVal, double stepSize){
        for(int i=0; i<10; i++) x[i] = initVal;
        for(int i=0; i<10; i++) step[i] = stepSize;
        for(int i=0; i<10; i++) gen[i].seed(rdArr[i]());
        MutationCount = 0;
        successfulMutationCount = 0;
        generator.seed(rd());
        normal_distribution<double> distribution(0.0, 1);
    }
    double fitness(){
        double sum = 0;
        for (int i=0; i<10; i++) sum = sum + x[i]*x[i];
        //printf("sum: %lf\n", sum);
        return sum;
    }
    
    void mutate(int type){
        if (type==1) {//normal
            for (int i=0; i<10; i++) x[i] = x[i] + distribution(generator);
        }
        else if(type==2){//uncorrelated
            for(int i=0; i<10; i++) {
                step[i] = step[i]*exp(tao*distribution(generator) + TAO*distribution(gen[i]));
                if(step[i]<stepSizeBound) step[i] = stepSizeBound;
            }
            for(int i=0; i<10; i++) x[i] = x[i] + step[i]*distribution(gen[i]);
        }
        else if(type==3){// 1/5-rule
            MutationCount++;
            double tmpFitness = this->fitness();
            if (successfulMutationCount/(MutationCount+0.0001)-0.00001 > 0.2) {
                for(int i=0; i<10; i++) {
                    step[i] = step[i]/oneFiveRuleAlpha;
                    if(step[i]<stepSizeBound) step[i] = stepSizeBound;
                }
            }
            else if(successfulMutationCount/(MutationCount+0.0001)+0.00001 <0.2){
                for(int i=0; i<10; i++) {
                    step[i] = step[i]*oneFiveRuleAlpha;
                    if(step[i]<stepSizeBound) step[i] = stepSizeBound;
                }
            }
            for(int i=0; i<10; i++) x[i] = x[i] + step[i]*distribution(gen[i]);
            if(tmpFitness>this->fitness()) successfulMutationCount++;
        }
        
    }
    
    void operator = (const individual &a){
        for(int i=0; i<10; i++) this->x[i] = a.x[i];
        for(int i=0; i<10; i++) this->step[i] = a.step[i];
    }
};


int main(int argc, const char * argv[]) {
    const double stepSizeInit = 0.1;
    individual ind(1, stepSizeInit);
    individual nxt(1, stepSizeInit); // 1, 1 are just dummies
    for (int i=0; i<10000000; i++) {
        if (ind.fitness()<0.005){
            printf("iteration: %d\n", i);
            break;
        }
        
        //(1+1)-normal
        /*
        nxt = ind;
        ind.mutate(1);
        if(ind.fitness()>nxt.fitness()) ind = nxt;
        */
        
        //(1,1)-normal
        /*
        ind.mutate(1);
        */
        
        //(1+1)-uncorrelated
        /*
        nxt = ind;
        ind.mutate(2);
        if(ind.fitness()>nxt.fitness()) ind = nxt;
        */
        
        
        //(1,1)-uncorrelated
        /*
        ind.mutate(2);
        */
        
        //(1+1)- 1/5
        /*
         nxt = ind;
         ind.mutate(3);
         if(ind.fitness()>nxt.fitness()) ind = nxt;
        */
        
        
        //(1,1)- 1/5
        
         ind.mutate(3);
        
        
        
        
        
        if (i%1000==0) printf("fitness: %lf\n", ind.fitness());
    }
    return 0;
}














