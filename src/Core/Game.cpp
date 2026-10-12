#include "Game.h"

#include "../Core/Enumutils.h"
#include "../Network/Protocol.h"

#include <iostream>
#include <vector>
#include <QJsonArray>

struct piece {
    int par;
    std::set<int> liberties;
    std::vector<int> group;
    int liberty() { return liberties.size(); }
} pieces[19*19+1];

int pos_to_int(int x, int y) {
    return x * 19 + y;
}

std::pair<int,int> int_to_pos (int x) {
    return std::make_pair(x / 19, x % 19);
}

int find_par(int x) {
    return x == pieces[x].par ? x : pieces[x].par = find_par(pieces[x].par);
}

bool Union(int u, int v) {
    int a = find_par(u);
    int b = find_par(v);
    if (a == b) return 0;

    if (pieces[a].liberties.size() < pieces[b].liberties.size()) std::swap(a, b);
    pieces[b].par = a;
    pieces[a].liberties.insert(pieces[b].liberties.begin(), pieces[b].liberties.end());
    pieces[b].liberties.clear();
    pieces[a].group.insert(pieces[a].group.end(), pieces[b].group.begin(), pieces[b].group.end());
    pieces[b].group.clear();
    return 1;
}


Game::Game(QObject* parent) : QObject(parent)
{ // init do it later
    m_board_state.resize(19, std::vector<int>(19, -1));

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            int current_idx = pos_to_int(i, j);
            pieces[current_idx].par = current_idx;
            pieces[current_idx].group = {current_idx};
            pieces[current_idx].liberties.clear();

            for (int d = 0; d < 4; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (ni >= 0 && ni < 19 && nj >= 0 && nj < 19) {
                    pieces[current_idx].liberties.insert(pos_to_int(ni, nj));
                }
            }
        }
    }
}

void Game::setNetworkClient(NetworkClient* networkClient)
{
    m_networkClient = networkClient;
}

QString Game::matchId() const
{
    return m_matchId;
}

int Game::playerIndex() const
{
    return m_playerIndex;
}

QString Game::opponentName() const
{
    return m_opponentName;
}

std::vector<std::vector<int>> Game::board_state()
{
    qDebug() << m_board_state;
    return m_board_state;
}

bool Game::is_valid_move(int x, int y)
{
    if (x < 0 || x >= 19 || y < 0 || y >= 19) return false;
    if (m_board_state[x][y] != -1) return false;

    int current_idx = pos_to_int(x, y);
    if (current_idx == m_ko_square) return false;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    bool empty_square = false;
    bool can_connect = false;
    int total_captured_stones = 0;

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
            int neighbor_color = m_board_state[nx][ny];

            if (neighbor_color == -1) {
                empty_square = true;
            }
            else if (neighbor_color == m_playerIndex) {
                int p = find_par(pos_to_int(nx, ny));
                if (pieces[p].liberties.size() > 1) {
                    can_connect = true;
                }
            }
            else {
                int p = find_par(pos_to_int(nx, ny));
                if (pieces[p].liberties.size() == 1 && pieces[p].liberties.count(current_idx)) {
                    total_captured_stones += pieces[p].group.size();
                }
            }
        }
    }

    if (!empty_square && !can_connect && total_captured_stones == 0) {
        return false;
    }

    return true;
}


void Game::update_pieces(int x, int y)
{
    if (x < 0 || x >= 19 || y < 0 || y >= 19) return;

    m_board_state[x][y] = m_playerIndex;
    int current_stone = pos_to_int(x, y);

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    m_board_state[x][y] = m_playerIndex;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
            if (m_board_state[nx][ny] != -1) {
                int neighbor_par = find_par(pos_to_int(nx, ny));
                pieces[neighbor_par].liberties.erase(current_stone);
            }
        }
    }

    pieces[current_stone].par = current_stone;
    pieces[current_stone].liberties.clear();
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
            if (m_board_state[nx][ny] == -1) {
                pieces[current_stone].liberties.insert(pos_to_int(nx, ny));
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < 19 && ny >= 0 && ny < 19) {
            if (m_board_state[nx][ny] == m_playerIndex) {
                Union(current_stone, pos_to_int(nx, ny));
            }
        }
    }

    pieces_updated();
    return;
}

void Game::setMatchInfo(const QString& matchId, int playerIndex, const QString& opponentName) {

}

void Game::handleServerMessage(const QJsonObject& message) {

}