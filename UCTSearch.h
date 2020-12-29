#ifndef UCTSEARCH_H_INCLUDED
#define UCTSEARCH_H_INCLUDED

#include <memory>
#include <atomic>
#include <tuple>

#include "GameState.h"
#include "UCTNode.h"
#include "Playout.h"

class UCTSearch {
public:
    /*
        Depending on rule set and state of the game, we might
        prefer to pass, or we might prefer not to pass unless
        it's the last resort. Same for resigning.
    */
    typedef int passflag_t;
    static const passflag_t NORMAL   = 0;
    static const passflag_t NOPASS   = 1 << 0;
    static const passflag_t NORESIGN = 1 << 1;

    /*
        Maximum size of the tree in memory.
    */
    static constexpr int MAX_TREE_SIZE = 10000000;

    UCTSearch(GameState & g);
    int think(int color, passflag_t passflag = NORMAL);
    void set_playout_limit(int playouts);
    void set_use_nets(bool usenets);
    void set_runflag(std::atomic<bool> * flag);
    void set_analyzing(bool flag);
    void set_quiet(bool flag);
    void ponder();
    bool is_running();
    bool playout_limit_reached();
    void increment_playouts();
    Playout play_simulation(KoState & currstate, UCTNode * const node);
    std::tuple<float, float, float> get_scores();

private:
    void dump_stats(KoState & state, UCTNode & parent);
    void dump_GUI_stats(GameState & state, UCTNode & parent);
    std::string get_pv(KoState & state, UCTNode & parent);
    void dump_thinking();
    void dump_analysis();
    void dump_order2(void);
    bool stop_thinking(int elapsed_centis = 0, int time_for_move = 0) const;
    int get_best_move(passflag_t passflag);
    int get_best_move_nosearch(std::vector<std::pair<float, int>> moves,
                               float score, passflag_t passflag);
    bool allow_early_exit();
    bool allow_easy_move();
    bool easy_move_precondition();
    void output_analysis(GameState & state, UCTNode & parent);

    GameState & m_rootstate;
    UCTNode m_root;
    std::atomic<int> m_nodes;
    std::atomic<int> m_playouts;
    std::atomic<bool> m_run;
    int m_maxplayouts;

    // For external control
    bool m_hasrunflag;
    std::atomic<bool> * m_runflag;

    // Special modes
    bool m_use_nets;
    bool m_analyzing;
    bool m_quiet;
};

class UCTWorker {
public:
    UCTWorker(GameState & state, UCTSearch * search, UCTNode * root)
      : m_rootstate(state), m_search(search), m_root(root) {};
    void operator()();
private:
    GameState & m_rootstate;
    UCTSearch * m_search;
    UCTNode * m_root;
};

#endif
