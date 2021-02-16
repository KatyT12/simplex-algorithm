#include "Simplex.h"
int main()
{
    int componentNum;
    int equationNum;
    std::cout << "Amount of components\n>";
    std::cin >> componentNum;
    std::cout << "Amount of equations\n>";
    std::cin >> equationNum;

    std::vector<Row> equations;

    for(int i =0;i<equationNum;i++)
    {
        std::vector<float>c;
        for(int i =0;i<componentNum;i++)
        {
            int num;
            std::cout << "\nInput component " << i << " >";
            std::cin >> num;
            c.push_back(num);
        }
        int num;
        std::cout << "\nInput Value >";
        std::cin >> num;
        c.push_back(num);

        equations.push_back(Row(c));
    }

    std::cout << "Now the objective function...\n";
    std::vector<float>c;
    for(int i =0;i<componentNum;i++)
    {
        int num;
        std::cout << "\nInput component " << i << " >";
        std::cin >> num;
        c.push_back(num);
    }
    c.push_back(0); //Value of objective

    Row objective(c);
    Table t (equations,objective);
    t.componentNum = componentNum;
    /*
    Coord p = t.findPivot();
    t.usePivotOnTable(p);
    t.print();
    */
    std::vector<float> vals = t.getMax();
    for(int i =0;i<vals.size()-1;i++)
    {
        std::cout << "Component " << i << " = " << vals[i] << "\n";
    }
    std::cout << "Max Profit: " << vals[vals.size()-1] << "\n";


}



