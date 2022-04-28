#pragma once
#include "source.h"

class Player
{
    private:
        int player_x,player_y,player_index,server_pid,campsite_x,campsite_y,round,deaths,coins_found,coins_brought;
        interface type;
        WINDOW *player_window;
        int discovered_campsite;   
        sem_t *si,*so;
    public:
Player(int x, int y, int i)
{
    initscr();
    player_window = newwin(27, 100,0,0);
    player_x = x;
    player_y = y;
    server_pid = i;
    keypad(player_window,1);
    discovered_campsite = 0;
    campsite_x = 0;
    campsite_y = 0;

    //pairing colors
	start_color();
	init_pair(PURPLE, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW);

    //other stuff
	cbreak();
	raw();
	noecho();
	curs_set(0);
	
    //maintenance
	wclear(player_window);
	refresh();
    wrefresh(player_window);

}

    ~Player()
    {
        sem_close(si);
        sem_close(so);
        endwin();
    }

    void set_sem(sem_t *s, int i)
    {
        if(i == 1) si = s;
        if(i == 2) so = s;
    }

    WINDOW *getwin()
    {
        return player_window;
    }

    void sync(Player_Stats p)
    {
        player_x = p.player_x;
        player_y = p.player_y;
        server_pid = p.PID;
        round = p.round;
        deaths = p.deaths;
        coins_found = p.coins_found;
        coins_brought = p.coins_brought;
        player_index = p.number;
        type = p.who;
    }

    void move_view(Player_Stats p)
    {
        int x,y;

        wclear(player_window);

        wrefresh(player_window);

        sync(p);

        
        for(int i = 0 ; i < 5 ; i++)
        {
            y = player_y - 2 + i;
            for(int j = 0 ; j < 5 ; j++)
            {
                x = player_x - 2 + j;
                switch(p.map[i * 5 + j])
                {
                    case ' ':
                    mvwprintw(player_window,y,x," ");
                    break;
                    case '#':
                    mvwprintw(player_window,y,x,"#");
                    break;
                    case WALL:
                    mvwprintw(player_window,y,x,"█");
                    break;
                    
                    default:
                    switch(CH_CHAR(p.map[i * 5 + j]))
                    {
                        case '1':
                        wattron(player_window, COLOR_PAIR(PURPLE));
                        mvwprintw(player_window,y,x,"1");
                        wattroff(player_window, COLOR_PAIR(PURPLE));
                        break;
                        case '2':
                        wattron(player_window, COLOR_PAIR(PURPLE));
                        mvwprintw(player_window,y,x,"2");
                        wattroff(player_window, COLOR_PAIR(PURPLE));
                        break;
                        case '3':
                        wattron(player_window, COLOR_PAIR(PURPLE));
                        mvwprintw(player_window,y,x,"3");
                        wattroff(player_window, COLOR_PAIR(PURPLE));
                        break;
                        case '4':
                        wattron(player_window, COLOR_PAIR(PURPLE));
                        mvwprintw(player_window,y,x,"4");
                        wattroff(player_window, COLOR_PAIR(PURPLE));
                        break;
                        case 'A':
                        if(discovered_campsite == 0)
                        {
                            discovered_campsite = 1;
                            campsite_y = player_y + i - 2;
                            campsite_x = player_x + j - 2;
                        }
                        
                        wattron(player_window, COLOR_PAIR(GREEN));
                        mvwprintw(player_window,y,x,"A");
                        wattroff(player_window, COLOR_PAIR(GREEN));
                        break;
                        case 'c':
                        wattron(player_window, COLOR_PAIR(YELLOW));
                        mvwprintw(player_window,y,x,"c");
                        wattroff(player_window, COLOR_PAIR(YELLOW));
                        break;
                        case 't':
                        wattron(player_window, COLOR_PAIR(YELLOW));
                        mvwprintw(player_window,y,x,"t");
                        wattroff(player_window, COLOR_PAIR(YELLOW));
                        break;
                        case 'T':
                        wattron(player_window, COLOR_PAIR(YELLOW));
                        mvwprintw(player_window,y,x,"T");
                        wattroff(player_window, COLOR_PAIR(YELLOW));
                        break;
                        case 'D':
                        wattron(player_window, COLOR_PAIR(YELLOW));
                        mvwprintw(player_window,y,x,"D");
                        wattroff(player_window, COLOR_PAIR(YELLOW));
                        break;
                        case '*':
                        wattron(player_window, COLOR_PAIR(RED));
                        mvwprintw(player_window,y,x,"*");
                        wattroff(player_window, COLOR_PAIR(RED));
                        break;
                    }
                    break;
                }
            } 
        }

        update_stats();

        draw_player(p.number);
        wrefresh(player_window);
   
    }
    void draw_player(int id)
    {
        wattron(player_window, COLOR_PAIR(PURPLE));
        mvwprintw(player_window,player_y,player_x,"%d",id + 1);
        wattroff(player_window, COLOR_PAIR(PURPLE));
    }

    void set_index(int ind)
    {
        player_index = ind;
    }

    int get_index()
    {
        return player_index;
    }
    void update_stats()
    {
        mvwprintw(player_window,0,57,"Server's PID: %d",server_pid);
        if(discovered_campsite)
        {
            mvwprintw(player_window,1,57,"Campsite X/Y: %d/%d",campsite_x - 1,campsite_y - 1);
        }
        else{
            mvwprintw(player_window,1,57,"Campsite X/Y: --/--");
        }
        
        mvwprintw(player_window,2,57,"Round number: %d",round);

        mvwprintw(player_window,4,57,"Player: ");
        mvwprintw(player_window,5,57,"Number:   %d",player_index + 1);
        switch(type)
        {
            case HUMAN:
            mvwprintw(player_window,6,57,"Type:     HUMAN");
            break;

            case CPU:
            mvwprintw(player_window,6,57,"CPU");
            break;
        }
        mvwprintw(player_window,7,57,"Curr X/Y: %d/%d",player_x - 1,player_y - 1);
        mvwprintw(player_window,8,57,"Deaths:   %d",deaths);

        mvwprintw(player_window,10,57,"Coins:");
        mvwprintw(player_window,11,57,"Coins carried: %d",coins_found);
        mvwprintw(player_window,12,57,"Coins brought: %d",coins_brought);

        mvwprintw(player_window,16,57,"Legend:");
        wattron(player_window, COLOR_PAIR(PURPLE));
        mvwprintw(player_window,17,57,"1234");
        wattroff(player_window, COLOR_PAIR(PURPLE));
        mvwprintw(player_window,17,61," - players"); 
        mvwprintw(player_window,18,57,"█    - wall");
        mvwprintw(player_window,19,57,"#    - bushes (slow down)");
        wattron(player_window, COLOR_PAIR(RED));
        mvwprintw(player_window,20,57,"*");
        wattroff(player_window, COLOR_PAIR(RED));
        mvwprintw(player_window,20,58,"    - enemy");
        wattron(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,21,57,"c");
        wattroff(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,21,58,"    - one coin");
        wattron(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,21,80,"D");
        wattroff(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,21,81,"    - dropped coins");
        wattron(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,22,57,"C");
        wattroff(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,22,58,"    - treasure (10 coins)");
        wattron(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,23,57,"T");
        wattroff(player_window, COLOR_PAIR(YELLOW));
        mvwprintw(player_window,23,58,"    - large treasure (50 coins)");
        wattron(player_window, COLOR_PAIR(GREEN));
        mvwprintw(player_window,24,57,"A");
        wattroff(player_window, COLOR_PAIR(GREEN));
        mvwprintw(player_window,24,58,"    - campsite (save coins)");
    }
};
