#include <iostream>
#include <vector>
#include <algorithm>

struct Coord
{
    int x;
    int y;
};


class Row{
    public:
        std::vector<float> components;
        Row(std::vector<float> c)
        {
            components = c;
        }
};

class Table{
    public:
        int* bv;

        int componentNum = 3;
        
        std::vector<Row> rows;
        Row objective;
        
        bool artificialVariables = false;
        Table(std::vector<Row> functions, Row objectiveFunction)
        : objective(objectiveFunction)
        {
            rows = functions;
            bv = new int[rows.size()];
        }
        
        void setArtificialFunction(Row* a)
        {
            rows.push_back(objective);
            objective = *a;
        }

        //This returns the values of all the components in order and the value of the optimised vale is last. Optimal Non integer solution
        std::vector<float> getMax()
        {
            while(!checkIfFinnished())
            {
                Coord p = findPivot();
                usePivotOnTable(p);
                checkBasicVariables();
            }
            if(artificialVariables)
            {
                objective = rows[rows.size()-1];
                rows.pop_back();
                while(!checkIfFinnished())
                {
                    Coord p = findPivot();
                    usePivotOnTable(p);
                    checkBasicVariables();
                }
            }
            return getFinishedValues();
        }

        float getValueAt(Coord c)
        {
            return rows[c.y].components[c.x];
        }

        void usePivotOnTable(Coord pivot)
        {
            int pivotRow = pivot.y;  
            float value = getValueAt(pivot);
            divideRow(pivotRow,value);
            for(int i = 0;i<rows.size();i++)
            {
                if(i == pivotRow)
                {
                    continue;
                }
                value = getValueAt({pivot.x,i});
                addRow(&rows[i],&rows[pivotRow],value * -1);
            }
            value = objective.components[pivot.x];
            addRow(&objective,&rows[pivotRow],value * -1);
        }

        void divideRow(int r, float val)
        {
            std::for_each(rows[r].components.begin(),rows[r].components.end(),[val](float& x){x /= val;});
        }

        void addRow(Row* r1,Row* r2, float coefficient)
        {
            std::vector<float>& c1 = r1->components;
            std::vector<float>& c2 = r2->components;

            for(int i = 0; i < componentNum+1;i++)
            {
                c1[i] += c2[i] * coefficient;
            }
        }

        bool checkIfFinnished()
        {
            for(int i =0; i < componentNum;i++)
            {
                if(objective.components[i] < 0)
                {
                    return false;
                }
            }
            return true;
        }

        void checkBasicVariables()
        {
            for(int i = 0; i < rows.size();i++)
            {
                for(int j = 0; j < componentNum;j++)
                {
                    if(getValueAt({j,i}) == 1)
                    {
                        bool isBasic = true;
                        for(int r = 0; r < rows.size();r++)
                        {
                            if(r == i)
                            {
                                continue;
                            }
                            if(rows[r].components[j] != 0)
                            {
                                isBasic = false;
                            }
                        }
                        if(objective.components[j] != 0)
                        {
                            isBasic = false;
                        }
                        
                        if(isBasic)
                        {
                            bv[i] = j;
                        }
                    }
                    else{
                            
                        }
                }    
            }
        }

        std::vector<float> getFinishedValues()
        {
            checkBasicVariables();
            std::vector<float> vals;
            for(int i = 0; i < componentNum;i++)
            {
                bool value = false;
                for(int r = 0; r < rows.size();r++)
                {
                    if(bv[r] == i)
                    {
                        value = true;
                        vals.push_back(rows[r].components[componentNum]);
                    }
                }
                if(!value)
                {
                    vals.push_back(0);
                }
            }
            vals.push_back(objective.components[componentNum]);

            return vals;
        }
        //For debugging
        void print()
        {
            for(int i =0;i<componentNum;i++)
            {
                std::cout << i << " ";
            }
            std::cout<< "Value \n";
            for(int i =0;i<rows.size();i++)
            {
                for(int j =0;j<componentNum+1;j++)
                {
                    std::cout << rows[i].components[j] << " ";
                }
                std::cout<< "\n";
            }

            std::for_each(objective.components.begin(),objective.components.end(),[](float x) {std::cout << x << " ";});
            std::cout<< "\n";
        }

        Coord findPivot()
        {
            Coord c;
            c.x = 0;
            int min = objective.components[0];
            for(int i = 1; i < componentNum;i++)
            {
                if(objective.components[i] < min)
                {
                    min = objective.components[i];
                    c.x = i;
                }
            }
            
            std::vector<float> theta;
            for(int i =0; i < rows.size();i++)
            {
                float val;
                val = rows[i].components[componentNum];

                val /= rows[i].components[c.x];
                theta.push_back(val);
            }
            
            c.y=0;
            min = 1000000000;
            for(int i = 0; i < rows.size();i++)
            {
                if(theta[i] < min && theta[i] > 0)
                {
                    min = theta[i];
                    c.y = i;
                }
            }
            return c;
        }
};
