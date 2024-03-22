
#include "kirk_patrick_seidel.h"
#include <bits/stdc++.h>
#include <iostream>
#include <limits>

using namespace std;

bool compareVector2(Vector2 a, Vector2 b)
{
    if (a.x == b.x)
        return a.y < b.y;
    return a.x < b.x;
}

float median_of_medians(vector<float> arr)
{
    int n = arr.size();
    vector<vector<float>> matrix(n / 5 + (n % 5 != 0));
    int i = 0, s = 0;

    while (i < n)
    {
        int count = 0;
        while (count < 5)
        {
            if (i < n)
            {
                matrix[s].push_back(arr[i]);
                i++;
            }
            else
                matrix[s].push_back(arr[n - 1]);
            count++;
        }
        s++;
    }

    for (vector<float> &V : matrix)
        sort(V.begin(), V.end());

    return matrix[matrix.size() / 2][2];
}

float quick_select(vector<float> S, int rank)
{
    if (S.size() < rank)
        return 0;
    float x = median_of_medians(S);
    int r = 0, dup = 0;
    vector<float> L, R;

    for (float e : S)
    {
        if (e < x)
        {
            L.push_back(e);
        }
        else if (e > x)
            R.push_back(e);
        else
            dup++;
    }
    dup--;
    r = L.size() + 1;

    if (r == rank || r < rank && (r + dup) >= rank)
        return x;
    else if (r > rank)
        return quick_select(L, rank);
    else
        return quick_select(R, rank - (r + dup));
}

vector<Vector2> upper_bridge(vector<Vector2> S, float L)
{
    int n = S.size();
    if (n <= 2)
        return S;

    vector<Vector2> candidates;

    // make pairs
    int pos = 0;
    vector<pair<int, int>> pairs;
    for (int i = 0; i < n / 2; i++)
    {
        int p_i = pos, p_j = n - 1 - pos;
        if (S[p_i].x > S[p_j].x)
        {
            p_i = p_j;
            p_j = pos;
        }

        pairs.push_back({p_i, p_j});
        pos++;
    }

    // point that could not be paired is added to candidates
    if (n % 2)
        candidates.push_back(S[pos]);

    // calculate slopes
    vector<pair<pair<int, int>, float>> slopes;
    vector<float> only_slopes;
    for (auto p : pairs)
    {
        if (S[p.first].x == S[p.second].x)
            candidates.push_back(max(S[p.first], S[p.second], compareVector2));
        else
        {
            slopes.push_back({p, (S[p.first].y - S[p.second].y) / (S[p.first].x - S[p.second].x)});
            only_slopes.push_back((S[p.first].y - S[p.second].y) / (S[p.first].x - S[p.second].x));
        }
    }

    // get median slope
    int k = slopes.size();

    float K = quick_select(only_slopes, k / 2 + 1);

    // calculate intersections
    vector<float> intersections;
    for (auto point : S)
        intersections.push_back(point.y - (K * point.x));

    Vector2 p_k, p_m;
    float m_int = -1 * numeric_limits<float>::infinity();

    for (int i = 0; i < n; i++)
    {
        if (intersections[i] - m_int > 0.0001)
        {
            m_int = intersections[i];
            p_k.x = S[i].x;
            p_k.y = S[i].y;
            p_m = p_k;
        }
        else if (abs(intersections[i] - m_int) <= 0.0001)
        {
            if (S[i].x > p_m.x)
            {
                p_m.x = S[i].x;
                p_m.y = S[i].y;
            }
            else if (S[i].x < p_k.x) // remove the else ? lite...
            {
                p_k.x = S[i].x;
                p_k.y = S[i].y;
            }
        }
    }

    // step 8 in slides
    if (p_k.x <= L && p_m.x > L)
        return {{p_k.x, p_k.y}, {p_m.x, p_m.y}};

    if (p_m.x <= L)
    {
        for (auto sl : slopes)
        {
            if (sl.second < K)
            {
                candidates.push_back(S[sl.first.first]);
                candidates.push_back(S[sl.first.second]);
            }
            else
                candidates.push_back(S[sl.first.second]);
        }
    }
    else if (p_k.x > L)
    {
        for (auto sl : slopes)
        {
            if (sl.second > K)
            {
                candidates.push_back(S[sl.first.first]);
                candidates.push_back(S[sl.first.second]);
            }
            else
                candidates.push_back(S[sl.first.first]);
        }
    }

    return upper_bridge(candidates, L);
}

vector<Vector2> upper_hull(vector<Vector2> S)
{
    int n = S.size();
    if (n <= 2)
    {
        sort(S.begin(), S.end(), compareVector2);
        return S;
    }

    // Get x_mid
    vector<float> x_s;
    for (auto v : S)
        x_s.push_back(v.x);
    float x_mid = quick_select(x_s, n / 2 + 1);
    float x__mid = quick_select(x_s, max(1, n / 2));
    x_mid = (x_mid + x__mid) / 2;
    x_mid = x_mid - 0.001; // IMPORTANT

    vector<Vector2> pq = upper_bridge(S, x_mid);
    sort(pq.begin(), pq.end(), compareVector2); // O(1) cause constant size

    vector<Vector2> L, R, res, temp_res;
    for (auto v : S)
    {
        if (v.x < pq[0].x)
            L.push_back(v);
        else if (v.x > pq[1].x)
            R.push_back(v);
        else if (pq[0].x != pq[1].x && v.x != pq[0].x &&
                 v.x != pq[1].x) // co-linear (lies btw p and q) but not vertical
        {
            float s1 = (pq[0].y - pq[1].y) / (pq[0].x - pq[1].x);
            float s2 = (pq[0].y - v.y) / (pq[0].x - v.x);
            if (abs(s1 - s2) < 0.0001) // s1 == s2
                temp_res.push_back(v);
        }
        else if (pq[0].x == pq[1].x && v.x == pq[0].x && v.y > pq[0].y &&
                 v.y < pq[1].y) // co-linear (lies btw p and q) and vertical
        {
            temp_res.push_back(v);
        }
    }
    L.push_back(pq[0]);
    R.push_back(pq[1]);

    sort(temp_res.begin(), temp_res.end(), compareVector2); // O(hlogh)

    L = upper_hull(L);
    R = upper_hull(R);

    for (auto v : L)
        res.push_back(v);
    if (!Vector2Equals(res.back(), pq[0]))
        res.push_back(pq[0]);
    for (auto v : temp_res) // contains all co-linear points on the bridge
        res.push_back(v);
    if (!Vector2Equals(R[0], pq[1]))
        res.push_back(pq[1]);
    for (auto v : R)
        res.push_back(v);

    return res;
}

vector<Vector2> lower_hull(vector<Vector2> &S) // same as upper hull with negative y co-ordinates
{
    vector<Vector2> S_new, res;
    for (auto v : S)
        S_new.push_back({v.x, -v.y});

    res = upper_hull(S_new);
    for (auto &v : res)
        v.y = -v.y;
    return res;
}

vector<Vector2> convex_hull(vector<Vector2> &S)
{
    if (S.size() <= 2)
        return S;
    vector<Vector2> uh, lh, res;
    uh = upper_hull(S);
    lh = lower_hull(S);

    // remove common points (with upper hull) from lower hull where x = x_max
    int r = uh.size() - 1;
    while (r >= 0 && Vector2Equals(lh.back(), uh[r]))
    {
        lh.pop_back();
        r--;
    }

    // at x = x_min, upper hull might have a point (x_min,y1) and lower hull have (x_min,y2)
    // however, if y1 < y2, we swap the points
    int s = 0;
    while (s < lh.size() && s < uh.size() && lh[s].y > uh[s].y)
    {
        swap(uh[s], lh[s]);
        s++;
    }

    // does similar things as the upper block at x = x_max
    s = 1;
    while (s <= lh.size() && s <= uh.size() && lh[lh.size() - s].x == uh[uh.size() - s].x &&
           lh[lh.size() - s].y > uh[uh.size() - s].y)
    {
        swap(uh[uh.size() - s], lh[lh.size() - s]);
        s++;
    }

    reverse(lh.begin(), lh.end()); // to make a chain in final result

    float x_min = numeric_limits<float>::infinity();
    float x_max = -1 * numeric_limits<float>::infinity();
    for (auto v : S)
    {
        if (v.x > x_max)
            x_max = v.x;
        if (v.x < x_min)
            x_min = v.x;
    }

    vector<Vector2> temp_res; // stores points vertically co-linear at either ends

    for (auto v : S)
    {
        if (v.x == x_max || v.x == x_min)
            temp_res.push_back(v);
    }
    sort(temp_res.begin(), temp_res.end(), compareVector2);

    // follwing block of code inserts into the result the points which are
    // vertically co-linear and between the first point in the upper hull and
    // last point in the (now reversed) lower hull
    // picture: these three form a vertical line at the left
    int i = 0;
    if (temp_res.size() > 2)
    {
        while (FloatEquals(temp_res[i].x, uh[0].x))
        {
            while (i < temp_res.size() && temp_res[i].y < uh[0].y && temp_res[i].y > lh.back().y &&
                   FloatEquals(temp_res[i].x, uh[0].x))
            {
                res.push_back(temp_res[i]);
                i++;
            }
            i++;
        }
    }

    // add upper hull to result
    for (auto v : uh)
    {
        if (res.size() == 0 || res.size() > 0 && !Vector2Equals(res.back(), v))
            res.push_back(v);
    }

    // the following block of code adds to the result, points vertically co-linear and between
    // the first point in (now reversed) lower hull and last point in upper hull
    // picture: these three form a vertical line at the right
    int p = 0;
    while (i < temp_res.size() && p < lh.size() && !Vector2Equals(temp_res[i], lh[p]))
    {
        i++;
        p++;
    }
    if (temp_res.size() > 2)
    {
        while (i < temp_res.size() && temp_res[i].y < uh.back().y && temp_res[i].y > lh[0].y &&
               temp_res[i].x == lh[0].x)
        {
            res.push_back(temp_res[i]);
            i++;
        }
    }

    // add lower hull to the result
    for (auto v : lh)
    {
        if (res.size() == 0 || res.size() > 0 && !Vector2Equals(res.back(), v))
            res.push_back(v);
    }

    // remove duplicates
    // example: we remove last 'a' from a->b->c->d->a
    int rr = 0;
    while (rr < res.size() && Vector2Equals(res[rr], res.back()))
    {
        res.pop_back();
        rr++;
    }

    return res;
}

int main()
{
    vector<Vector2> P = {{1, 1}, {2, 2}, {2, 0}, {2, 4}, {3, 3}, {4, 2}};
    vector<Vector2> res = convex_hull(P);
    for (auto h : res)
    {
        cout << "( " << h.x << ", " << h.y << " ) ,";
    }
    cout << endl;
    return 0;
}