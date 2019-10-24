#include "route_rule_based.h"
#include <algorithm>
#include <list>
#include <cstring>
#include <queue>

using std::sin;
using std::cos;

const int delta_x[] = { 0, 1, -1, 0 };
const int delta_y[] = { 1, 0, 0, -1 };

RoutePtr get_rule_based_algo()
{
    return std::make_shared<RouteRuleBasedImpl>();
}

void RouteRuleBasedImpl::rotate(double &x, double &y, double angle)
{
    double nx = x * cos(angle) - y * sin(angle);
    double ny = x * sin(angle) + y * cos(angle);
    x = nx, y = ny;
}

double RouteRuleBasedImpl::evaluate_to_edge(double x, double y)
{
    x -= center_x, y -= center_y;
    rotate(x, y);
    x += center_x, y += center_y;
    return std::min(std::min(x, y), std::min(grid_x() - 1 - x, grid_y() - 1 - y));
}

int RouteRuleBasedImpl::evaluate_to_point(int x, int y, Vertex T)
{
    return std::abs(x - T.x) + std::abs(y - T.y);
}

int RouteRuleBasedImpl::best_direction(int x, int y)
{
    double f = 1e10;
    int d[4] = {0}, tag = 0;
    for (int k = 0; k < 4; ++k)
    {
        double tmp = evaluate_to_edge(x + delta_x[k], y + delta_y[k]);
        if (tmp < f)f = tmp, d[tag = 0] = k;
        else if (tmp == f)d[++tag] = k;
    }
    return d[rand() % (tag + 1)];
}

int RouteRuleBasedImpl::calc_no_target_route(Vertex S)
{
    int now_x, now_y;
    int min_dis = 1 << 30;
    auto cmp = [&](const Vertex & a, const Vertex & b) -> bool
    {return distance[a.x][a.y] + evaluate_to_edge(a.x, a.y) > distance[b.x][b.y] + evaluate_to_edge(b.x, b.y);};
    std::priority_queue<Vertex, std::vector<Vertex>, decltype(cmp)>que(cmp);
    auto check = [&](int x, int y, int d) -> bool
    {
        if (dir[x][y] >= 4)
            return false;
        if (dir[x][y] == -1)
            if (visit[x][y] != time_ ||
            visit[x][y] == time_ && distance[x][y] > 1 + distance[now_x][now_y])
            {
                visit[x][y] = time_;
                dir_tmp[x][y] = d;
                distance[x][y] = distance[now_x][now_y] + 1;
                que.push((Vertex){x, y});
                return true;
            }
        return false;
    };
    distance[S.x][S.y] = 0;
    visit[S.x][S.y] = time_++;
    int next_d = best_direction(S.x, S.y);
    que.push(S);
    while (que.size())
    {
        now_x = que.top().x, now_y = que.top().y;
        que.pop();
        if (now_x == 0 || now_x == grid_x() - 1 || now_y == 0 || now_y == grid_y() - 1)
        {
            min_dis = distance[now_x][now_y];
            break;
        }
        int next_x = now_x + delta_x[next_d];
        int next_y = now_y + delta_y[next_d];
        check(next_x, next_y, next_d);
        for (int k = 0; k < 4; k++)
            if ((k ^ 3) != next_d && k != next_d)
            {
                next_x = now_x + delta_x[k];
                next_y = now_y + delta_y[k];
                check(next_x, next_y, k);
            }
    }
    return min_dis != 1 << 30;
}

bool RouteRuleBasedImpl::label_dir (const int &check_case, Vertex S, Vertex T)
{
    //0: erase; 1: label; 2: check
    Vertex tmp = T;
    while (tmp.x != S.x || tmp.y != S.y)
    {
        //check if there is a source point without linked
        if (check_case == 2)
            for (int k = 0; k < 4; ++k)
            {
                int nx = tmp.x + delta_x[k], ny = tmp.y + delta_y[k];
                if (dir[nx][ny] == 5 && calc_no_target_route((Vertex){nx, ny}) == 0)
                return 0;
            }
        int d = dir_tmp[tmp.x][tmp.y];
        Vertex last = (Vertex){tmp.x - delta_x[d], tmp.y - delta_y[d]};
        if (check_case == 1)dir[last.x][last.y] = d;
        else if (check_case == 0)dir[last.x][last.y] = dir_bak[last.x][last.y];
        tmp = last;
    }
    return check_case > 0;
}
int RouteRuleBasedImpl::calc_route(Vertex S, Vertex T)
{
    //	printf("(%d,%d)->(%d,%d)\n", S.x, S.y, T.x, T.y);
    int now_x, now_y;
    int min_dis = 1 << 30;
    auto cmp = [&](const Vertex & a, const Vertex & b) -> bool
    {return distance[a.x][a.y] + 2 * evaluate_to_point(a.x, a.y, T) > distance[b.x][b.y] + 2 * evaluate_to_point(b.x, b.y, T);};
    std::priority_queue<Vertex, std::vector<Vertex>, decltype(cmp)>que(cmp);
    auto check = [&](int x, int y, int d) -> bool
    {
        if (x < 0 || x >= grid_x() || y < 0 || y >= grid_y())
            return false;
        if (dir[x][y] >= 4)
            return false;
        if (dir[x][y] == -1)
            if (visit[x][y] != time_ ||
            visit[x][y] == time_ && distance[x][y] > 1 + distance[now_x][now_y])
            {
                visit[x][y] = time_;
                dir_tmp[x][y] = d;
                distance[x][y] = distance[now_x][now_y] + 1;
                que.push((Vertex){x, y});
                return true;
            }
        return false;
    };
    dir[S.x][S.y] = -1;
    dir[T.x][T.y] = -1;
    distance[S.x][S.y] = 0;
    visit[S.x][S.y] = ++time_;
    int next_d = 0;//best_direction(S.x, S.y);
    que.push(S);
    while (que.size())
    {
        now_x = que.top().x, now_y = que.top().y;
        que.pop();
        if (now_x == T.x && now_y == T.y)
        {
            min_dis = distance[now_x][now_y];
            break;
        }
        int next_x = now_x + delta_x[next_d];
        int next_y = now_y + delta_y[next_d];
        check(next_x, next_y, next_d);
        for (int k = 0; k < 4; k++)
            if ((k ^ 3) != next_d && k != next_d)
            {
                next_x = now_x + delta_x[k];
                next_y = now_y + delta_y[k];
                check(next_x, next_y, k);
            }
    }
    if (min_dis == 1 << 30)
    {
        //		std::printf("Error in (%d, %d)\n", S.x, S.y);
        total_error_node++;
        return 0;
    }
    dir[T.x][T.y] = 4;
    label_dir(1, S, T);
    if (label_dir(2, S, T) == 0)
    {
        label_dir(0, S, T);
        dir[T.x][T.y] = dir_bak[T.x][T.y];
        return 0;
    }
    return 1;
}

int RouteRuleBasedImpl::calc_midean(int *res, int n) //0~n-1
{
    int a[std::max(node_x(), node_y()) + 10] = {0};
    for (int i = 0; i < n; i++)
        a[i] = res[i];
    int l = 0, r = n - 1;
    while (r - l > 1)
    {
        int tmp = std::min(a[l], a[r]);
        if (a[l] -= tmp, a[l] == 0)l++;
        if (a[r] -= tmp, a[r] == 0)r--;
    }
    return r;
}

void RouteRuleBasedImpl::calc_00_dir(const int case_)
{
    const int dbg = 0;
    //for all best_dir==0
    //target: (*,node_y_-1)
    int res[std::max(node_x(), node_y()) + 10] = {0}, top[std::max(node_x(), node_y()) + 10] = {0}, low[std::max(node_x(), node_y()) + 10] = {0};
    bool link[std::max(grid_x(), grid_y()) + 10] = {0};
    if (dbg)printf("interval=%d\n", interval);
    //1st step: link all edge points
    for (int i = 0; i < node_x(); ++i)
        if (label[i][node_y() - 1])
        {
            int gx, gy;
            node2grid(i, node_y() - 1, gx, gy);
            calc_route((Vertex){gx, gy}, (Vertex){gx, grid_y() - 1});
            link[gx] = 1;
            top[i] = -1;
            low[i] = 1000;
        }
    //2nd step: calc mid interval
    for (int i = 0; i < node_x(); ++i)
        for (int j = 0; j < node_y() - 1; ++j)
            if (label[i][j])
            {
                res[i]++;
                top[i] = std::max(top[i], j);
                low[i] = std::min(low[i], j);
            }
    int now_range = calc_midean(res, node_x());
    if (node_x() % 2 == 0)now_range = node_x() / 2;
    if (dbg)puts("3rd");
    //3rd step: link center points to an edge, passing midean interval
    {
        std::vector<Vertex> midean_points;
        while (midean_points.size() < interval)
            if (res[now_range - 1] && (low[now_range - 1] < low[now_range] || case_ && low[now_range - 1] <= low[now_range]))
            {
                midean_points.push_back((Vertex){now_range - 1, low[now_range - 1]});
                low[now_range - 1]++;
                res[now_range - 1]--;
            }
            else if (res[now_range])
            {
                midean_points.push_back((Vertex){now_range, low[now_range]});
                low[now_range]++;
                res[now_range]--;
            }
            else break;
        int l = node2grid((Vertex){now_range - 1, 0}).x + 1;
        int r = node2grid((Vertex){now_range, 0}).x - 1;
        while (midean_points.size())
        {
            Vertex now = midean_points.back();
            midean_points.pop_back();
            if (now.x == now_range)
            {
                calc_route(node2grid(now), (Vertex){r, grid_y() - 1});
                r--;
            }
            else
            {
                calc_route(node2grid(now), (Vertex){l, grid_y() - 1});
                l++;
            }
        }
    }
    if (dbg)puts("4thL");
    int left = now_range, right = now_range - 1, tmp1, tmp2;
    //4th step: calculate left half points' solution
    for (int index, temp = node2grid((Vertex){now_range - 1, 0}).x - 1;;)
    {
        while (right >= 0 && res[right] == 0)right--;
        if (right < 0 || node2grid((Vertex){right, 0}).x < temp - 1)break;
        for (index = right; index >= 0; index--)
            if (res[index] > 0 && calc_route(node2grid((Vertex){index, top[index]}), (Vertex){temp, grid_y() - 1}))break;
        if (index < 0)break;
        top[index]--;
        res[index]--;
        link[temp] = 1;
        while (dir[temp][grid_y() - 1] >= 4)temp--;
        tmp1 = temp;
    }
    if (dbg)puts("4thR");
    //calculate right half points' solution
    for (int index, temp = node2grid((Vertex){now_range, 0}).x + 1;;)
    {
        while (left <= node_x() - 1 && res[left] == 0)left++;
        if (left > node_x() - 1 || node2grid((Vertex){left, 0}).x > temp + 1)break;
        for (index = left; index < node_x(); index++)
            if (res[index] > 0 && calc_route(node2grid((Vertex){index, top[index]}), (Vertex){temp, grid_y() - 1}))break;
        if (index == node_x())break;
        top[index]--;
        res[index]--;
        link[temp] = 1;
        while (dir[temp][grid_y() - 1] >= 4)temp++;
        tmp2 = temp;
    }
    if (dbg)puts("4th end");
    //5th step: fix fringe on both side
    for (; right >= 0; right--)
        for (; res[right] > 0; res[right]--)
        {
            int l, r, tmp;
            tmp = l = r = node2grid((Vertex){right, 0}).x;
            while (l >= 0 && link[l])l--;
            while (r < grid_y() && link[r])r++;
            tmp = (tmp - l <= r - tmp ? l : r);
            calc_route(node2grid((Vertex){right, top[right]}), (Vertex){tmp, grid_y() - 1});
            top[right]--;
            link[tmp] = 1;
        }
    for (; left < node_x(); left++)
        for (; res[left] > 0; res[left]--)
        {
            int l, r, tmp;
            tmp = l = r = node2grid((Vertex){left, 0}).x;
            while (l >= 0 && link[l])l--;
            while (r < grid_y() && link[r])r++;
            tmp = (tmp - l <= r - tmp ? l : r);
            calc_route(node2grid((Vertex){left, top[left]}), (Vertex){tmp, grid_y() - 1});
            top[left]--;
            link[tmp] = 1;
        }
}

void RouteRuleBasedImpl::calc_0_dir(const int case_)
{
    dir = dir_bak;
    for (int i = 0; i < node_x(); ++i)
        for (int j = 0; j < node_y(); ++j)
            label[i][j] = (label_bak[i][j] == 0);
    calc_00_dir(case_);
    for (int i = 0; i < grid_x(); ++i)
        for (int j = 0; j < grid_y(); ++j)
            if (dir[i][j] >= 0 && dir[i][j] <= 4)
                dir_ans[i][j] = dir[i][j];
}

void RouteRuleBasedImpl::calc_3_dir(const int case_)
{
    dir = dir_bak;
    for (int i = 0; i < node_x(); ++i)
        for (int j = 0; j < node_y(); ++j)
            label[i][node_y() - 1 - j] = (label_bak[i][j] == 3);
    calc_00_dir(case_);
    for (int i = 0; i < grid_x(); ++i)
        for (int j = 0; j < grid_y(); ++j)
            if (dir[i][j] >= 0 && dir[i][j] <= 4)
                dir_ans[i][grid_y() - 1 - j] = (dir[i][j] == 0 || dir[i][j] == 3 ? dir[i][j] ^ 3 : dir[i][j]);
}

void RouteRuleBasedImpl::cw_dir(VecList_short &dir_ans, VecList_short &dir)
{
    const int change_dir[] = {1, 3, 0, 2, 4};
    for (int i = 0; i < grid_x(); ++i)
        for (int j = 0; j < grid_y(); ++j)
            if (dir[i][j] >= 0 && dir[i][j] <= 4)
                dir_ans[j][grid_y() - 1 - i] = change_dir[dir[i][j]];
}

void RouteRuleBasedImpl::ccw_dir(VecList_short &dir_ans, VecList_short &dir)
{
    const int change_dir[] = {2, 0, 3, 1, 4};
    for (int i = 0; i < grid_x(); ++i)
        for (int j = 0; j < grid_y(); ++j)
            if (dir[i][j] >= 0 && dir[i][j] <= 4)
                dir_ans[grid_x() - 1 - j][i] = change_dir[dir[i][j]];
}
void RouteRuleBasedImpl::calc_1_dir(const int case_)
{
    dir = dir_bak;
    if (case_ == 0)ccw_dir(dir, dir_ans);
    for (int i = 0; i < node_x(); ++i)
        for (int j = 0; j < node_y(); ++j)
            label[node_x() - 1 - j][i] = (label_bak[i][j] == 1);
    calc_00_dir(case_);
    cw_dir(dir_ans, dir);
}

void RouteRuleBasedImpl::calc_2_dir(const int case_)
{
    dir = dir_bak;
    if (case_ == 0)cw_dir(dir, dir_ans);
    for (int i = 0; i < node_x(); ++i)
        for (int j = 0; j < node_y(); ++j)
            label[j][node_y() - 1 - i] = (label_bak[i][j] == 2);
    calc_00_dir(case_);
    ccw_dir(dir_ans, dir);
}

Path RouteRuleBasedImpl::compute_path(int x, int y, int _case)
{
    int ox = x, oy = y;
    int dir_ = best_direction(x, y);
    Path path;
    if(!_case)
    	return path;
    path.append({x, y});
    label_bdr[x][y] = dir_;
    while (dir_ans[x][y] < 4 && dir_ans[x][y] >= 0)
    {
        int d = dir_ans[x][y];
        x += delta_x[d];
        y += delta_y[d];
        path.append({x, y});
        label_bdr[x][y] = dir_;
    }
    int __tmp__ = label_show[ox][oy] = std::min(std::abs(x - center_x), std::abs(y - center_y));
    max_time = std::max(max_time, __tmp__);
    return path;
}

void RouteRuleBasedImpl::init_array()
{
    time_ = 0;
    int size_dir = std::max(grid_x(), grid_y()) + 10;
    dir.assign(size_dir, ShortList(size_dir, 0));
    dir_bak.assign(size_dir, ShortList(size_dir, -1));
    dir_tmp.assign(size_dir, ShortList(size_dir, 0));
    dir_ans.assign(size_dir, ShortList(size_dir, -1));
    distance.assign(size_dir, IntList(size_dir, 1 << 30));
    visit.assign(size_dir, IntList(size_dir, 0));
    label_bdr.assign(size_dir, ShortList(size_dir, -1));
    label_show.assign(size_dir, IntList(size_dir, 1 << 30));

    int size_label = std::max(node_x(), node_y()) + 10;
    label.assign(size_label, ShortList(size_label, 0));
    label_bak.assign(size_label, ShortList(size_label, 0));
}

void RouteRuleBasedImpl::compute(CallbackFunc callback)
{
    compute_grid_size(grid_x_, grid_y_);
    if (grid_x_ == -1)
    {
        std::puts("Grid size is too large!");
        return;
    }
    else
    {
        std::printf("Found grid size: %dx%d\n", grid_x_, grid_y_);
    }

    init_array();

    interval = node2grid((Vertex){1, 0}).x - node2grid((Vertex){0, 0}).x - 1;
    center_x = (grid_x() - 1) * .5, center_y = (grid_y() - 1) * .5;

    for (int i = 0; i != node_x(); ++i)
        for (int j = 0; j != node_y(); ++j)
        {
            int gx, gy;
            _node2grid(i, j, gx, gy);
            label_bak[i][j] = best_direction(gx, gy);
            dir_ans[gx][gy] = dir_bak[gx][gy] = 5;
        }
    calc_0_dir();
    calc_3_dir();
    calc_1_dir();
    calc_2_dir();
    path_.clear();
    for (int i = 0; i != node_x(); ++i)
        for (int j = 0; j != node_y(); ++j)
        {
            int gx, gy;
            node2grid(i, j, gx, gy);
            path_.push_back(compute_path(gx, gy, 1));
        }

    if(callback)
    {
        for(int _T = 0; _T <= max_time; ++_T)
        {
            path_.clear();
            for (int i = 0; i != node_x(); ++i)
            {
                for (int j = 0; j != node_y(); ++j)
                {
                    int gx, gy;
                    _node2grid(i, j, gx, gy);
                    path_.push_back(compute_path(gx, gy, label_show[gx][gy] <= _T));
                }
            }
            callback(this);
        }
    }
    //	std::cout << "Error = " << total_error_node << std::endl;
    //	std::cout << "length by NF: " << lowlowdeans << std::endl;
    //	std::cout << node_x() << " : " << length() << std::endl;
    std::cout << "\rFinished, cost = " << length() << std::endl;

    if (!_validate())
        std::cerr << "ERROR!" << std::endl;
}
