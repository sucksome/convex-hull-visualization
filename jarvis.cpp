#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vector2
{
    int x, y;
};

int orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

vector<Vector2> computeConvexHull(vector<Vector2> points)
{
    int n = points.size();
    vector<Vector2> convexHull;

    if (n < 3)
    {
        return points;
    }

    int left = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].x < points[left].x)
        {
            left = i;
        }
        else if (points[i].x == points[left].x && points[i].y < points[left].y)
        {
            left = i;
        }
    }
    int current = left, next;

    do
    {
        convexHull.push_back(points[current]);
        next = (current + 1) % n;

        for (int i = 0; i < n; i++)
        {
            if (orientation(points[current], points[i], points[next]) == 2)
            {
                next = i;
            }
        }

        current = next;
    } while (current != left);
    return convexHull;
}

int main()
{
    // vector<Vector2> points = {
    //     {0, 20},  {0, 22},  {0, 35},  {0, 37},  {0, 40},  {0, 56},  {0, 57},
    //     {0, 58},  {0, 69},  {0, 77},  {0, 83},  {0, 99},  {1, 4},   {1, 8},
    //     {1, 37},  {1, 65},  {1, 77},  {1, 81},  {1, 90},  {1, 96},  {1, 98},
    //     {2, 5},   {2, 15},  {2, 19},  {2, 27},  {2, 32},  {2, 34},  {2, 41},
    //     {2, 56},  {2, 65},  {2, 66},  {2, 98},  {3, 33},  {3, 38},  {3, 45},
    //     {3, 52},  {3, 62},  {3, 65},  {3, 83},  {3, 95},  {4, 0},   {4, 4},
    //     {4, 28},  {4, 30},  {4, 35},  {4, 43},  {4, 67},  {4, 84},  {4, 95},
    //     {5, 3},   {5, 4},   {5, 8},   {5, 33},  {5, 71},  {5, 78},  {5, 79},
    //     {5, 82},  {5, 89},  {5, 91},  {6, 7},   {6, 25},  {6, 26},  {6, 31},
    //     {6, 71},  {6, 86},  {6, 93},  {6, 94},  {7, 2},   {7, 31},  {7, 35},
    //     {7, 39},  {7, 44},  {7, 54},  {7, 70},  {8, 8},   {8, 17},  {8, 18},
    //     {8, 24},  {8, 27},  {8, 29},  {8, 39},  {8, 65},  {8, 66},  {8, 69},
    //     {8, 73},  {8, 93},  {9, 7},   {9, 15},  {9, 35},  {9, 69},  {9, 73},
    //     {9, 84},  {9, 93},  {9, 97},  {10, 15}, {10, 24}, {10, 25}, {10, 61},
    //     {10, 68}, {10, 82}, {11, 16}, {11, 22}, {11, 25}, {11, 41}, {11, 52},
    //     {11, 56}, {12, 4},  {12, 8},  {12, 12}, {12, 30}, {12, 49}, {12, 53},
    //     {12, 67}, {12, 69}, {12, 97}, {12, 99}, {13, 11}, {13, 27}, {13, 33},
    //     {13, 34}, {13, 55}, {13, 58}, {13, 59}, {13, 84}, {13, 98}, {14, 9},
    //     {14, 45}, {14, 48}, {14, 50}, {14, 65}, {14, 80}, {14, 86}, {15, 1},
    //     {15, 5},  {15, 8},  {15, 13}, {15, 16}, {15, 18}, {15, 23}, {15, 39},
    //     {15, 49}, {15, 52}, {15, 60}, {15, 61}, {15, 63}, {15, 64}, {15, 72},
    //     {15, 89}, {15, 97}, {16, 3},  {16, 5},  {16, 9},  {16, 27}, {16, 48},
    //     {16, 60}, {16, 74}, {16, 77}, {16, 79}, {16, 84}, {16, 87}, {16, 95},
    //     {17, 3},  {17, 10}, {17, 25}, {17, 33}, {17, 48}, {17, 67}, {17, 73},
    //     {17, 75}, {17, 82}, {17, 83}, {17, 86}, {17, 87}, {17, 98}, {18, 17},
    //     {18, 18}, {18, 46}, {18, 55}, {18, 59}, {18, 61}, {18, 66}, {18, 68},
    //     {18, 83}, {18, 85}, {19, 0},  {19, 42}, {19, 49}, {19, 55}, {19, 95},
    //     {20, 2},  {20, 10}, {20, 18}, {20, 29}, {20, 35}, {20, 54}, {20, 69},
    //     {20, 91}, {20, 93}, {21, 7},  {21, 11}, {21, 22}, {21, 40}, {21, 67},
    //     {22, 3},  {22, 4},  {22, 15}, {22, 31}, {22, 49}, {22, 58}, {22, 68},
    //     {22, 91}, {22, 94}, {22, 96}, {23, 1},  {23, 16}, {23, 21}, {23, 26},
    //     {23, 29}, {23, 69}, {23, 78}, {23, 86}, {24, 8},  {24, 18}, {24, 22},
    //     {24, 39}, {24, 42}, {24, 46}, {24, 53}, {24, 62}, {24, 64}, {24, 73},
    //     {24, 86}, {24, 92}, {24, 98}, {25, 8},  {25, 20}, {25, 54}, {25, 58},
    //     {25, 86}, {25, 88}, {25, 91}, {26, 6},  {26, 7},  {26, 18}, {26, 28},
    //     {26, 64}, {26, 67}, {26, 79}, {26, 89}, {26, 94}, {26, 99}, {27, 11},
    //     {27, 13}, {27, 23}, {27, 97}, {28, 14}, {28, 30}, {28, 39}, {28, 41},
    //     {28, 43}, {28, 81}, {28, 98}, {29, 23}, {29, 29}, {29, 50}, {29, 68},
    //     {29, 74}, {29, 81}, {29, 99}, {30, 18}, {30, 21}, {30, 31}, {30, 57},
    //     {30, 78}, {30, 87}, {31, 2},  {31, 37}, {31, 58}, {31, 65}, {31, 82},
    //     {31, 83}, {32, 7},  {32, 13}, {32, 18}, {32, 27}, {32, 40}, {32, 51},
    //     {32, 53}, {32, 68}, {32, 84}, {32, 90}, {32, 97}, {33, 2},  {33, 3},
    //     {33, 38}, {33, 57}, {33, 68}, {33, 75}, {33, 77}, {34, 9},  {34, 11},
    //     {34, 14}, {34, 15}, {34, 25}, {34, 27}, {34, 44}, {34, 56}, {34, 57},
    //     {34, 61}, {34, 64}, {34, 71}, {34, 74}, {34, 75}, {35, 19}, {35, 21},
    //     {35, 22}, {35, 31}, {35, 33}, {35, 41}, {35, 43}, {35, 48}, {35, 64},
    //     {35, 69}, {35, 74}, {35, 87}, {36, 20}, {36, 23}, {36, 41}, {36, 42},
    //     {36, 77}, {36, 99}, {37, 6},  {37, 22}, {37, 28}, {37, 31}, {37, 59},
    //     {37, 81}, {37, 90}, {37, 95}, {37, 97}, {38, 3},  {38, 22}, {38, 24},
    //     {38, 42}, {38, 66}, {38, 68}, {38, 80}, {38, 87}, {39, 9},  {39, 14},
    //     {39, 16}, {39, 26}, {39, 55}, {39, 58}, {39, 60}, {39, 72}, {40, 18},
    //     {40, 29}, {40, 34}, {40, 37}, {40, 46}, {40, 56}, {40, 75}, {40, 83},
    //     {41, 0},  {41, 6},  {41, 28}, {41, 63}, {42, 11}, {42, 31}, {42, 47},
    //     {42, 52}, {42, 58}, {42, 59}, {42, 60}, {42, 64}, {42, 82}, {43, 2},
    //     {43, 9},  {43, 14}, {43, 19}, {43, 50}, {43, 56}, {43, 60}, {43, 77},
    //     {43, 78}, {43, 80}, {43, 86}, {43, 87}, {43, 88}, {43, 91}, {44, 34},
    //     {44, 41}, {44, 69}, {44, 77}, {44, 86}, {44, 93}, {45, 2},  {45, 7},
    //     {45, 14}, {45, 21}, {45, 48}, {45, 50}, {45, 54}, {45, 56}, {45, 65},
    //     {45, 77}, {45, 87}, {45, 88}, {46, 3},  {46, 9},  {46, 13}, {46, 19},
    //     {46, 26}, {46, 29}, {46, 37}, {46, 44}, {46, 69}, {46, 79}, {46, 91},
    //     {46, 96}, {47, 1},  {47, 9},  {47, 11}, {47, 15}, {47, 28}, {47, 36},
    //     {47, 51}, {47, 61}, {47, 62}, {47, 64}, {47, 67}, {47, 75}, {47, 80},
    //     {47, 82}, {47, 86}, {47, 91}, {47, 94}, {47, 99}, {48, 1},  {48, 19},
    //     {48, 34}, {48, 38}, {48, 41}, {48, 90}, {48, 91}, {48, 92}, {49, 2},
    //     {49, 3},  {49, 6},  {49, 8},  {49, 10}, {49, 14}, {49, 31}, {49, 49},
    //     {49, 62}, {49, 68}, {49, 70}, {49, 81}, {50, 6},  {50, 31}, {50, 38},
    //     {50, 50}, {50, 54}, {50, 84}, {50, 88}, {50, 92}, {50, 94}, {51, 6},
    //     {51, 51}, {51, 66}, {51, 71}, {51, 89}, {51, 90}, {51, 92}, {51, 94},
    //     {51, 98}, {52, 8},  {52, 12}, {52, 47}, {52, 52}, {52, 59}, {52, 67},
    //     {52, 79}, {53, 14}, {53, 28}, {53, 42}, {53, 51}, {53, 54}, {53, 75},
    //     {53, 89}, {54, 16}, {54, 25}, {54, 33}, {54, 36}, {54, 43}, {54, 63},
    //     {54, 69}, {54, 88}, {55, 0},  {55, 2},  {55, 9},  {55, 14}, {55, 16},
    //     {55, 27}, {55, 30}, {55, 36}, {55, 45}, {55, 97}, {55, 99}, {56, 7},
    //     {56, 20}, {56, 27}, {56, 29}, {56, 42}, {56, 49}, {56, 53}, {56, 72},
    //     {56, 91}, {56, 98}, {57, 4},  {57, 7},  {57, 15}, {57, 36}, {57, 39},
    //     {57, 49}, {57, 52}, {57, 56}, {57, 81}, {57, 82}, {57, 85}, {57, 95},
    //     {57, 99}, {58, 17}, {58, 19}, {58, 21}, {58, 34}, {58, 36}, {58, 43},
    //     {58, 45}, {58, 50}, {58, 62}, {58, 93}, {59, 1},  {59, 3},  {59, 13},
    //     {59, 28}, {59, 43}, {59, 82}, {60, 1},  {60, 2},  {60, 4},  {60, 6},
    //     {60, 16}, {60, 28}, {60, 34}, {60, 38}, {60, 53}, {60, 61}, {60, 65},
    //     {60, 71}, {60, 76}, {60, 90}, {60, 95}, {61, 1},  {61, 17}, {61, 19},
    //     {61, 30}, {61, 34}, {61, 35}, {61, 36}, {61, 48}, {61, 55}, {61, 60},
    //     {61, 73}, {61, 90}, {61, 92}, {61, 95}, {62, 3},  {62, 16}, {62, 24},
    //     {62, 45}, {62, 53}, {62, 56}, {62, 61}, {62, 62}, {62, 80}, {62, 82},
    //     {63, 65}, {63, 72}, {63, 82}, {64, 4},  {64, 8},  {64, 48}, {64, 79},
    //     {64, 89}, {64, 94}, {65, 14}, {65, 21}, {65, 24}, {65, 27}, {65, 34},
    //     {65, 48}, {65, 50}, {65, 51}, {65, 52}, {65, 56}, {65, 62}, {65, 66},
    //     {65, 74}, {65, 81}, {65, 82}, {65, 86}, {65, 89}, {66, 1},  {66, 12},
    //     {66, 15}, {66, 19}, {66, 27}, {66, 32}, {66, 48}, {66, 49}, {66, 53},
    //     {66, 59}, {66, 68}, {66, 77}, {66, 83}, {66, 89}, {66, 92}, {67, 21},
    //     {67, 24}, {67, 30}, {67, 32}, {67, 36}, {67, 42}, {67, 62}, {67, 68},
    //     {67, 73}, {67, 79}, {67, 87}, {67, 88}, {67, 92}, {67, 94}, {67, 98},
    //     {68, 16}, {68, 25}, {68, 42}, {68, 53}, {68, 68}, {68, 70}, {68, 72},
    //     {68, 73}, {68, 99}, {69, 6},  {69, 17}, {69, 25}, {69, 27}, {69, 41},
    //     {69, 49}, {69, 66}, {69, 67}, {70, 4},  {70, 23}, {70, 24}, {70, 25},
    //     {70, 32}, {70, 33}, {70, 41}, {70, 45}, {70, 49}, {70, 50}, {70, 54},
    //     {70, 64}, {70, 71}, {70, 77}, {70, 87}, {70, 90}, {70, 92}, {70, 97},
    //     {71, 6},  {71, 11}, {71, 12}, {71, 24}, {71, 29}, {71, 32}, {71, 51},
    //     {71, 54}, {71, 75}, {71, 85}, {71, 92}, {72, 34}, {72, 50}, {72, 53},
    //     {72, 64}, {72, 88}, {72, 89}, {72, 99}, {73, 1},  {73, 6},  {73, 12},
    //     {73, 22}, {73, 26}, {73, 32}, {73, 37}, {73, 45}, {73, 48}, {73, 64},
    //     {73, 65}, {73, 67}, {73, 72}, {73, 76}, {73, 82}, {73, 94}, {73, 96},
    //     {74, 1},  {74, 2},  {74, 4},  {74, 5},  {74, 32}, {74, 36}, {74, 55},
    //     {74, 61}, {74, 63}, {74, 83}, {74, 99}, {75, 5},  {75, 43}, {75, 59},
    //     {75, 65}, {75, 81}, {75, 83}, {76, 6},  {76, 23}, {76, 34}, {76, 55},
    //     {76, 76}, {76, 86}, {76, 89}, {76, 94}, {76, 97}, {77, 6},  {77, 21},
    //     {77, 24}, {77, 33}, {77, 34}, {77, 65}, {77, 86}, {78, 2},  {78, 10},
    //     {78, 14}, {78, 19}, {78, 22}, {78, 62}, {78, 74}, {78, 75}, {78, 83},
    //     {78, 87}, {78, 91}, {78, 92}, {78, 96}, {78, 99}, {79, 6},  {79, 24},
    //     {79, 25}, {79, 35}, {79, 38}, {79, 55}, {79, 56}, {79, 62}, {79, 75},
    //     {79, 86}, {80, 10}, {80, 27}, {80, 30}, {80, 54}, {80, 55}, {80, 59},
    //     {80, 93}, {80, 94}, {80, 96}, {81, 11}, {81, 25}, {81, 26}, {81, 30},
    //     {81, 56}, {81, 85}, {81, 93}, {81, 98}, {81, 99}, {82, 11}, {82, 17},
    //     {82, 18}, {82, 24}, {82, 37}, {82, 59}, {82, 67}, {82, 84}, {82, 87},
    //     {83, 3},  {83, 8},  {83, 24}, {83, 26}, {83, 29}, {83, 31}, {83, 41},
    //     {83, 46}, {83, 49}, {83, 52}, {83, 54}, {83, 58}, {83, 60}, {83, 62},
    //     {83, 91}, {84, 10}, {84, 21}, {84, 47}, {84, 50}, {84, 51}, {84, 52},
    //     {85, 1},  {85, 9},  {85, 16}, {85, 17}, {85, 27}, {85, 37}, {85, 65},
    //     {85, 68}, {85, 93}, {85, 98}, {86, 1},  {86, 10}, {86, 17}, {86, 19},
    //     {86, 28}, {86, 66}, {86, 82}, {87, 22}, {87, 23}, {87, 25}, {87, 32},
    //     {87, 71}, {87, 77}, {88, 3},  {88, 7},  {88, 14}, {88, 27}, {88, 39},
    //     {88, 49}, {88, 54}, {88, 59}, {88, 68}, {88, 83}, {88, 88}, {89, 0},
    //     {89, 7},  {89, 10}, {89, 19}, {89, 34}, {89, 51}, {89, 52}, {89, 59},
    //     {89, 61}, {89, 64}, {89, 70}, {89, 75}, {89, 89}, {89, 94}, {90, 3},
    //     {90, 22}, {90, 24}, {90, 32}, {90, 35}, {90, 39}, {90, 40}, {90, 49},
    //     {90, 56}, {90, 59}, {90, 67}, {90, 74}, {90, 77}, {90, 81}, {90, 82},
    //     {90, 92}, {90, 95}, {91, 1},  {91, 11}, {91, 30}, {91, 31}, {91, 45},
    //     {91, 55}, {91, 60}, {91, 74}, {91, 84}, {91, 92}, {91, 94}, {91, 98},
    //     {92, 2},  {92, 12}, {92, 29}, {92, 31}, {92, 45}, {92, 60}, {92, 87},
    //     {92, 96}, {93, 12}, {93, 31}, {93, 36}, {93, 43}, {93, 56}, {93, 93},
    //     {93, 94}, {93, 99}, {94, 1},  {94, 18}, {94, 20}, {94, 23}, {94, 27},
    //     {94, 38}, {94, 43}, {94, 44}, {94, 53}, {94, 66}, {95, 12}, {95, 17},
    //     {95, 20}, {95, 71}, {95, 74}, {95, 96}, {95, 98}, {96, 27}, {96, 39},
    //     {96, 41}, {96, 56}, {96, 71}, {96, 75}, {97, 2},  {97, 17}, {97, 18},
    //     {97, 41}, {97, 56}, {97, 65}, {97, 70}, {97, 87}, {97, 89}, {97, 91},
    //     {98, 10}, {98, 30}, {98, 35}, {98, 45}, {98, 48}, {98, 63}, {98, 77},
    //     {98, 80}, {98, 95}, {99, 4},  {99, 14}, {99, 19}, {99, 24}, {99, 40},
    //     {99, 42}, {99, 43}, {99, 44}, {99, 52}, {99, 79}, {99, 81}, {99, 84}};
    vector<Vector2> points;
    const std::string inputFilePath = "./in.txt";
    std::ifstream istream(inputFilePath);
    int x, y;
    char openParenthesis = '(', closeParenthesis = ')', separator = ',';
    while (istream >> openParenthesis >> x >> separator >> y >> closeParenthesis)
    {
        points.push_back({x, y});
    }
    auto result = computeConvexHull(points);
    const std::string outputFilePath = "./jarvisout.txt";
    std::ofstream ostream(outputFilePath);
    for (auto &point : result)
    {
        cout << "(" << point.x << ", " << point.y << ")" << endl;
        ostream << openParenthesis << point.x << separator << point.y << closeParenthesis << endl;
    }
}
