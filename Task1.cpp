/*
    Author: Rafiqul Islam
*/

#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#define SIZE 501
#define PI 3.1415926
#define DIFF 0.01f
#define DURATION 1.0 //assume 1 second
using namespace std;

class Wave{
    public:
        double frequency, amplitude, phase_offset;
        Wave(double frequency, double amplitude, double phase_offset){
            this->frequency=frequency;
            this->amplitude=amplitude;
            this->phase_offset=phase_offset;
        }

        virtual void show()=0;
        virtual void wave_generation_and_sampling()=0;
        virtual void show_sampled_output()=0;
};

class SineWave: public Wave{
    public:
        double sampling_freq; //For example: 200 Hz (for a duration of 1 second)
        double time_interval;
        int sample_index=0;
        vector<double>buffer_sine;
        SineWave(double freq, double amp, double phase, double sampling_freq):Wave(freq,amp,phase){
            this->sampling_freq=sampling_freq;
        }


        void wave_generation_and_sampling(){
            double theta=0.0;
            double t=0.0;
            time_interval=1/sampling_freq; //time interval between samples
            if(phase_offset==90){
                theta=PI/2.0; // sine 90 works as cosine
            }
            for(int i=0;i<SIZE;i++){
                double result= amplitude*sin((2*PI*frequency*t)+theta);
                buffer_sine.push_back(result);
                t+=time_interval;

            }

        }

        void show_sampled_output(){
            cout<<"----------------------------------------------------------------"<<endl;
            cout<<"Sine Wave Output"<<endl;
            cout<<"Frequency of "<<frequency<<"Hz  "<<"Sampling frequency of "<<sampling_freq<<"Hz Duration of "<<DURATION<<" Second"<<endl;
            cout<<"----------------------------------------------------------------"<<endl;
            double t=0.0;
            for(int i=0;i<SIZE;i++){
                cout<<buffer_sine[i]<<endl;
                t+=time_interval;
                if(fabs(t-(DURATION+time_interval))<DIFF){ // result shows only for 1 seconds
                    sample_index=i; //sampling index
                    break;
                }
            }
        }

        void show(){
            cout<<"------Sine Wave---------"<<endl;
            cout<<"Frequency: "<<frequency<<endl;
            cout<<"Amplitude: "<<amplitude<<endl;
            cout<<"Phase Offset: "<<phase_offset<<endl;
            cout<<"Sampling Frequency: "<<sampling_freq<<endl;
        }
};




class SquareWave: public Wave{
    public:
        double sampling_freq;
        double time_interval;
        int sample_index=0;
        vector<double>buffer_square;
        SquareWave(double freq, double amp, double phase, double sampling_freq):Wave(freq,amp,phase){
            this->sampling_freq=sampling_freq;
        }

        //Sign function
        double sign(double value){
            if(value>0.0){
                return 1.0*(this->amplitude);
            }
            if(value<0.0){
                return -1.0*(this->amplitude);
            }
        }

        void wave_generation_and_sampling(){
            double theta=0.0;
            double t=0.0;
            time_interval=1/sampling_freq; //time interval between samples
            if(phase_offset==0){
                theta=0;
            }
            if(phase_offset==90){
                theta=PI/2.0; // sine 90 works as cosine
            }
            for(int i=0;i<SIZE;i++){
                double result= amplitude*sin((2*PI*frequency*t)+theta);
                result=sign(result);
                buffer_square.push_back(result);
                t+=time_interval;


            }

        }

        void show_sampled_output(){
            cout<<"----------------------------------------------------------------"<<endl;
            cout<<"Square Wave Output"<<endl;
            cout<<"Frequency of "<<frequency<<"Hz  "<<"Sampling frequency of "<<sampling_freq<<"Hz Duration of "<<DURATION<<" Second"<<endl;
            cout<<"----------------------------------------------------------------"<<endl;

            double t=0.0;
            for(int i=0;i<SIZE;i++){
                cout<<buffer_square[i]<<endl;
                t+=time_interval;
                if(fabs(t-(DURATION+time_interval))<DIFF){ // Result shows for 1 seconds
                    sample_index=i; //sampling index
                    break;
                }

            }
        }



        void show(){
            cout<<"------Square Wave---------"<<endl;
            cout<<"Frequency: "<<frequency<<endl;
            cout<<"Amplitude: "<<amplitude<<endl;
            cout<<"Phase Offset: "<<phase_offset<<endl;
            cout<<"Sampling Frequency: "<<sampling_freq<<endl;
        }
};




int main(){

    //Sine Wave
    SineWave sine_wave(10,3,90,200); //frequency, amplitude, phase offset, sampling frequency
    Wave *sine=&sine_wave;
    sine->wave_generation_and_sampling();
    sine->show_sampled_output();

    //Square Wave
    SquareWave square_wave(40,1,0,200); //frequency, amplitude, phase offset, sampling frequency
    Wave *square=&square_wave;
    square->wave_generation_and_sampling();
    square->show_sampled_output();


    int max_len=max(sine_wave.sample_index,square_wave.sample_index); //sine and square wave that have different number of sampling frequency
                                        //will generate different number of sample in 1 second. In that case, take the maximum one
    //Mixed Signal
    vector<double>mixed_signal;
    for(int i=0;i<max_len;i++){
        mixed_signal.push_back(sine_wave.buffer_sine[i]*square_wave.buffer_square[i]);
    }
    cout<<"----------------------------------------------"<<endl;
    cout<<"Mixed Signal Output"<<endl;
    cout<<"----------------------------------------------"<<endl;
    for(int i=0;i<max_len;i++){
        cout<<mixed_signal[i]<<endl;
    }


    //Mean of the mixed signal
    int first_quarter=(max_len/4); //number of sample in first 0.25 second
    int sum=0;
    for(int i=0;i<first_quarter;i++){
        sum+=mixed_signal[i];
    }
    cout<<"\nMean of the mixed signal in the first 0.25 second: "<<((double)sum/first_quarter )<<endl;


    //Integral of the mixed signal
    int second_quarter=first_quarter*2;
    sum=0;
    for(int i=first_quarter;i<second_quarter;i++){
        sum+=mixed_signal[i];
    }

    cout<<"\nIntegral of the second quarter: "<<sum<<endl;

    return 0;
}

