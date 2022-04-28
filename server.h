#pragma once
#include "source.h"

class Server
{
    private:
        int PID;
        int campsite_x;
        int campsite_y;
        int round;
        int player_count;
        int window_height, window_width, start_y, start_x;
        int online_status[4] = {0};
        int is_on_bush[4];
        int is_on_campsite[4];
        int PIDS[4];
        int beast_spawn_flag;
        beasts beast[MAX_NUM_OF_BEASTS];
        int number_of_beasts;
        sem_t *SEMS[12];
        WINDOW *board;
        map<pair<int,int>,int>m;
        char server_map[27][53];
    public:
    Player_Stats players[4];
Server()
{
    initscr();

    //initializing server parameters
    update_id();
    beast_spawn_flag = 0;
    beast_start();
    campsite_x = 23;
    campsite_y = 11;
    window_width = 113;
    window_height = 27;
    start_x = 0;
    start_y = 0;
    round = 1;
    player_count = 0;
    players[0]={PID,0,0,0,0,0,0,0,0,NONE,(wchar_t)NULL};
    players[1]={PID,0,0,0,0,0,0,0,0,NONE,(wchar_t)NULL};
    players[2]={PID,0,0,0,0,0,0,0,0,NONE,(wchar_t)NULL};
    players[3]={PID,0,0,0,0,0,0,0,0,NONE,(wchar_t)NULL};
    is_on_bush[0] = 0;
    is_on_bush[1] = 0;
    is_on_bush[2] = 0;
    is_on_bush[3] = 0;
    is_on_campsite[0] = 0;
    is_on_campsite[1] = 0;
    is_on_campsite[2] = 0;
    is_on_campsite[3] = 0;
    PIDS[0] = 0;
    PIDS[1] = 0;
    PIDS[2] = 0;
    PIDS[3] = 0;
    
    board = newwin(window_height, window_width, start_y, start_x);

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
	wclear(board);
	refresh();

    //creating box
	box(board, 0, 0);
	wrefresh(board);

    //drawing map
    draw_game_v2();
    wrefresh(board);


}

~Server()
{
    for(int i = 0 ; i < 12 ; i++)
    {
        sem_close(SEMS[i]);
    }
    endwin();
};

    void set_sems(sem_t *s, int i)
    {
        SEMS[i] = s;
    }
    WINDOW *get_server_window()
    {
        return board;
    }

    wchar_t get_symbol(int x, int y)
    {
        return mvwinch(board, y, x);
    }

    int spawn_player(int number)
    {
        if(number < 0 || number > 3)   return 1;
        if(get_status(number) == 0)
        {
            int x = rand()%50 + 2, y = rand()%24 + 2;
            while(get_symbol(x,y) != ' ')
            {
                x = rand()%50 + 2;
                y = rand()%24 + 2;
            }
            players[number] = {number, x, y, 0, 0, 0,0,0,number, HUMAN};
            online_status[number] = 1;
            player_count++;

            return 0;
        }
        int x = rand()%50 + 2, y = rand()%24 + 2;
        while(get_symbol(x,y) != ' ')
        {
            x = rand()%50 + 2;
            y = rand()%24 + 2;
        }
        players[number].player_x = x;
        players[number].player_y = y;
        players[number].deaths++;
        players[number].number = number;

        return 1;
    }

    void update_id()
    {
        PID = getpid();
    }

    int get_pids(int i)
    {
        return PIDS[i];
    }

    void draw_game_v2()
    {
        wclear(board);
        server_map[campsite_y + 1][campsite_x + 1] = 'A';
        for(int i = 0 ; i < 27 ; i++)
        {
            for(int j = 0 ; j < 53 ; j++)
            {
                if(server_map[i][j] == 'O')
                {
                    mvwprintw(board,i,j,"█");
                }
                else if(server_map[i][j] == ' ')
                {
                    mvwprintw(board,i,j," ");
                }
                else if(server_map[i][j] == '1')
                {
                    wattron(board, COLOR_PAIR(PURPLE));
                    mvwprintw(board,i,j,"1");
                    wattroff(board, COLOR_PAIR(PURPLE));
                }
                else if(server_map[i][j] == '2')
                {
                    wattron(board, COLOR_PAIR(PURPLE));
                    mvwprintw(board,i,j,"2");
                    wattroff(board, COLOR_PAIR(PURPLE));
                }
                else if(server_map[i][j] == '3')
                {
                    wattron(board, COLOR_PAIR(PURPLE));
                    mvwprintw(board,i,j,"3");
                    wattroff(board, COLOR_PAIR(PURPLE));
                }
                else if(server_map[i][j] == '4')
                {
                    wattron(board, COLOR_PAIR(PURPLE));
                    mvwprintw(board,i,j,"4");
                    wattroff(board, COLOR_PAIR(PURPLE));
                }
                else if(server_map[i][j] == 'c')
                {
                    wattron(board, COLOR_PAIR(YELLOW));
                    mvwprintw(board,i,j,"c");
                    wattroff(board, COLOR_PAIR(YELLOW));
                }
                else if(server_map[i][j] == 't')
                {
                    wattron(board, COLOR_PAIR(YELLOW));
                    mvwprintw(board,i,j,"t");
                    wattroff(board, COLOR_PAIR(YELLOW));
                }
                else if(server_map[i][j] == 'T')
                {
                    wattron(board, COLOR_PAIR(YELLOW));
                    mvwprintw(board,i,j,"T");
                    wattroff(board, COLOR_PAIR(YELLOW));
                }
                
                else if(server_map[i][j] == 'A')
                {
                    wattron(board, COLOR_PAIR(GREEN));
                    mvwprintw(board,i,j,"A");
                    wattroff(board, COLOR_PAIR(GREEN));
                }
                else if(server_map[i][j] == '#')
                {
                    mvwprintw(board,i,j,"#");
                }
            }
        }
        draw_dead_treasure();
        draw_beasts();
        draw_players();
        update_stats();
        wrefresh(board);
    }

    void draw_dead_treasure()
    {
        
        for(int i = 0 ; i < 27 ; i++)
        {
            for(int j = 0 ; j < 53 ; j++)
            {
                if(m.count(make_pair(j, i)))
                {
                    wattron(board, COLOR_PAIR(YELLOW));
                    mvwprintw(board,i,j,"D");
                    wattroff(board, COLOR_PAIR(YELLOW));
                }
            }
        }
    }

    int get_status(int i)
    {
        return online_status[i];
    }

    void set_status(int i)
    {
        online_status[i] = 1;
    }

    void set_off_status(int i)
    {
        online_status[i] = 0;
    }

    void draw_players()
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(online_status[i])
            {
                wattron(board, COLOR_PAIR(PURPLE));
                mvwprintw(board,(players[i]).player_y,(players[i]).player_x,"%d",i + 1);
                wattroff(board, COLOR_PAIR(PURPLE));
            }
        }
        
    }

    int up(int id)
    {
        char res = server_map[players[id].player_y- 1][players[id].player_x];
        if(res != 'O')
        {
            if(is_on_campsite[id])
            {
                server_map[players[id].player_y][players[id].player_x] = 'A';
                is_on_campsite[id] = 0;
            }
            (players[id]).player_y--; 
            return 1;
        }
        return 0; 
    }

    int down(int id)
    {
        char res = server_map[players[id].player_y+ 1][players[id].player_x];
        if(res != 'O')
        {
            if(is_on_campsite[id])
            {
                server_map[players[id].player_y][players[id].player_x] = 'A';
                is_on_campsite[id] = 0;
            }
            (players[id]).player_y++; 
            return 1;
        }
        return 0; 
    }

    int left(int id)
    {
        char res = server_map[players[id].player_y][players[id].player_x - 1];
        if(res != 'O')
        {
            if(is_on_campsite[id])
            {
                server_map[players[id].player_y][players[id].player_x] = 'A';
                is_on_campsite[id] = 0;
            }
            (players[id]).player_x--; 
            return 1;
        }
        return 0; 
    }

    int right(int id)
    {
        char res = server_map[players[id].player_y][players[id].player_x + 1];
        if(res != 'O')
        {
            if(is_on_campsite[id])
            {
                server_map[players[id].player_y][players[id].player_x] = 'A';
                is_on_campsite[id] = 0;
            } 
            (players[id]).player_x++; 
            return 1;
        }
        return 0; 
    }

    void stand(int id)
    {
        mvwaddch(board,(players[id]).player_y,(players[id]).player_x,'i');
    }
    
    int get_player_count()
    {
        return player_count;
    }

    void move_player(int id, int input)
    {
        if(is_waiting(id))
        {
            stop_waiting(id);
            return;
        }
        if(input == KEY_UP || input == 'w')
        {
            if(up(id))
                decide_on_action(stepped_on(players[id].player_x,players[id].player_y),id);
            draw_game_v2();
        } 
        else if(input == KEY_DOWN || input == 's')  
        {
            if(down(id))
                decide_on_action(stepped_on(players[id].player_x,players[id].player_y),id);
            draw_game_v2();
        }
        else if(input == KEY_LEFT || input == 'a')  
        {
            if(left(id))
                decide_on_action(stepped_on(players[id].player_x,players[id].player_y),id);
            draw_game_v2();
        }
        else if(input == KEY_RIGHT || input == 'd') 
        {
            if(right(id))
                decide_on_action(stepped_on(players[id].player_x,players[id].player_y),id);
            draw_game_v2();
        }
        else if(input == 'q' || input == 'Q')
        {
            kill_player(id);
        }
        else if(input == 'b' || input == 'B')
        {
            beast_spawn_flag = 1;
        }
        else if(input == SPAWN_ME)
        {
            if(get_status(id) == 0)
            {
                spawn_player(id); 
            }
        }
        else if(input == 'c' || input == 't' || input == 'T')
        {
            spawn_coin(input);
        }
    }

    int get_round()
    {
        return round;
    }

    void server_next_round()
    {
        round++;
    }
    int get_x(int id)
    {
        return players[id].player_x;
    }

    int get_y(int id)
    {
        return players[id].player_y;
    }

    void spawn_coin(int c)
    {
        int x = rand()%50 + 2, y = rand()%24 + 2;
        while(get_symbol(x,y) != ' ')
        {
            x = rand()%50 + 2;
            y = rand()%24 + 2;
        }
        server_map[y][x] = c;
    }

    wchar_t stepped_on(int x, int y)
    {
        return mvwinch(board,y,x);
    }

    void decide_on_action(wchar_t c, int id)
    {
        switch(c)
        {
            case '#':
            players[id].waiting = 1;
            is_on_bush[id] = 1;
            return;
            
            default:
            switch(CH_CHAR(c))
            {
                case '1':
                if(id != 0) 
                {
                    kill_players(id,0);
                }

                return;

                case '2':
                if(id != 1) 
                {
                    kill_players(id,1);
                }
                return;

                case '3':
                if(id != 2) 
                {
                    kill_players(id,2);
                }
                return;

                case '4':
                if(id != 3) 
                {
                    kill_players(id,3);
                }
                return;

                case 'A':
                is_on_campsite[id] = 1;
                deposit(id);
                return;

                case 'c':
                players[id].coins_found += 1;
                server_map[players[id].player_y][players[id].player_x] = ' ';
                return;

                case 't':
                players[id].coins_found += 10;
                server_map[players[id].player_y][players[id].player_x] = ' ';
                return;

                case 'T':
                players[id].coins_found += 50;
                server_map[players[id].player_y][players[id].player_x] = ' ';
                return;

                case 'D':
                players[id].coins_found += m.find(make_pair(players[id].player_x, players[id].player_y))->second;
                m.erase(make_pair(players[id].player_x, players[id].player_y));
                return;

                case '*':
                return;

                default:
                return;
            }
            return;
        }
    } 
    void deposit(int id)
    {
        players[id].coins_brought += players[id].coins_found;
        players[id].coins_found = 0;
    }
    int is_waiting(int id)
    {
        if(players[id].waiting == 1)
            return 1;
        else return 0;
    }
    void stop_waiting(int id)
    {
        players[id].waiting = 0;
    }

    void spawn_dead_treasure(int id, int id2)
    {
        if(players[id].coins_found + players[id2].coins_found > 0)
        {
            int sum = players[id].coins_found + players[id2].coins_found;
            m[make_pair(players[id].player_x,players[id].player_y)] = sum;
            players[id].coins_found = 0;
            players[id2].coins_found = 0;
        }
    }

    void spawn_dead_treasure_single(int id)
    {
        if(players[id].coins_found> 0)
        {
            int sum = players[id].coins_found;
            m[make_pair(players[id].player_x,players[id].player_y)] = sum;
            players[id].coins_found = 0;
        }
    }

    void kill_players(int id1, int id2)
    {
        spawn_dead_treasure(id1,id2);
        spawn_player(id1);
        spawn_player(id2);
    }

    void kill_player_beast(int id)
    {
        spawn_dead_treasure_single(id);
        spawn_player(id);
    }

    void set_player_PID(int id, int PIDp)
    {
        PIDS[id] = PIDp;
    }

    void update_stats()
    {
        mvwprintw(board,1,80,"Server's PID: %d",PID);
        mvwprintw(board,1,57,"Campsite X/Y: %d/%d",23,11);
        mvwprintw(board,2,57,"Round number: %d",round);


        mvwprintw(board,4,57,"Player:");
        //todo
        mvwprintw(board,5,57,"PID:");
        //
        mvwprintw(board,6,57,"Type:");
        mvwprintw(board,7,57,"Curr X/Y:");
        mvwprintw(board,8,57,"Deaths:");

        mvwprintw(board,10,54,"Coins:");
        mvwprintw(board,11,54,"Coins carried:");
        mvwprintw(board,12,54,"Coins brought:");

        //PLAYER 1//
        mvwprintw(board,4,68,"Player 1:");
        //todo
        mvwprintw(board,5,68,"   ");
        mvwprintw(board,5,68,"%d",PIDS[0]);
        //
        switch(players[0].who)
        {
            case HUMAN:
            mvwprintw(board,6,68,"HUMAN");
            break;

            case CPU:
            mvwprintw(board,6,68,"CPU");
            break;

            case NONE:
            mvwprintw(board,6,68,"-----");
            break;
        }
        mvwprintw(board,7,68,"  /  ");
        mvwprintw(board,7,68,"%2d/%2d",players[0].player_x - 1,players[0].player_y - 1);
        mvwprintw(board,8,68,"%d",players[0].deaths);

        mvwprintw(board,11,68,"     ");
        mvwprintw(board,11,68,"%d",players[0].coins_found);
        mvwprintw(board,12,68,"     ");
        mvwprintw(board,12,68,"%d",players[0].coins_brought);


        //PLAYER 2//
        mvwprintw(board,4,79,"Player 2:");
        //todo
        mvwprintw(board,5,79,"%d",PIDS[1]);
        //
        switch(players[1].who)
        {
            case HUMAN:
            mvwprintw(board,6,79,"HUMAN");
            break;

            case CPU:
            mvwprintw(board,6,79,"CPU");
            break;

            case NONE:
            mvwprintw(board,6,79,"-----");
            break;
        }
        mvwprintw(board,7,79,"  /  ");
        mvwprintw(board,7,79,"%2d/%2d",players[1].player_x - 1,players[1].player_y - 1);
        mvwprintw(board,8,79,"%d",players[1].deaths);

        mvwprintw(board,11,79,"     ");
        mvwprintw(board,11,79,"%d",players[1].coins_found);
        mvwprintw(board,12,79,"     ");
        mvwprintw(board,12,79,"%d",players[1].coins_brought);

        //PLAYER 3//
        mvwprintw(board,4,90,"Player 3:");
        //todo
        mvwprintw(board,5,90,"%d",PIDS[2]);
        //
        switch(players[2].who)
        {
            case HUMAN:
            mvwprintw(board,6,90,"HUMAN");
            break;

            case CPU:
            mvwprintw(board,6,90,"CPU");
            break;

            case NONE:
            mvwprintw(board,6,90,"-----");
            break;
        }
        mvwprintw(board,7,90,"  /  ");
        mvwprintw(board,7,90,"%2d/%2d",players[2].player_x - 1,players[2].player_y - 1);
        mvwprintw(board,8,90,"%2d",players[2].deaths);

        mvwprintw(board,11,90,"     ");
        mvwprintw(board,11,90,"%d",players[2].coins_found);
        mvwprintw(board,12,90,"     ");
        mvwprintw(board,12,90,"%d",players[2].coins_brought);

        //PLAYER 4//
        mvwprintw(board,4,101,"Player 4:");
        //todo
        mvwprintw(board,5,101,"%d",PIDS[3]);
        //
        switch(players[3].who)
        {
            case HUMAN:
            mvwprintw(board,6,101,"HUMAN");
            break;

            case CPU:
            mvwprintw(board,6,101,"CPU");
            break;

            case NONE:
            mvwprintw(board,6,101,"-----");
            break;
        }
        mvwprintw(board,7,101,"  /  ");
        mvwprintw(board,7,101,"%2d/%2d",players[3].player_x - 1,players[3].player_y - 1);
        mvwprintw(board,8,101,"%d",players[3].deaths);

        mvwprintw(board,11,101,"     ");
        mvwprintw(board,11,101,"%d",players[3].coins_found);
        mvwprintw(board,12,101,"     ");
        mvwprintw(board,12,101,"%d",players[3].coins_brought);
        

        mvwprintw(board,16,57,"Legend:");
        wattron(board, COLOR_PAIR(PURPLE));
        mvwprintw(board,17,57,"1234");
        wattroff(board, COLOR_PAIR(PURPLE));
        mvwprintw(board,17,61," - players"); 
        mvwprintw(board,18,57,"█    - wall");
        mvwprintw(board,19,57,"#    - bushes (slow down)");
        wattron(board, COLOR_PAIR(RED));
        mvwprintw(board,20,57,"*");
        wattroff(board, COLOR_PAIR(RED));
        mvwprintw(board,20,58,"    - enemy");
        wattron(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,21,57,"c");
        wattroff(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,21,58,"    - one coin");
        wattron(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,21,80,"D");
        wattroff(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,21,81,"    - dropped coins");
        wattron(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,22,57,"C");
        wattroff(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,22,58,"    - treasure (10 coins)");
        wattron(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,23,57,"T");
        wattroff(board, COLOR_PAIR(YELLOW));
        mvwprintw(board,23,58,"    - large treasure (50 coins)");
        wattron(board, COLOR_PAIR(GREEN));
        mvwprintw(board,24,57,"A");
        wattroff(board, COLOR_PAIR(GREEN));
        mvwprintw(board,24,58,"    - campsite (save coins)");
    }
    void load_map()
    {
        FILE *f = fopen("map.txt","r");
        char trash;
        for(int i = 0 ; i < 27 ; i++)
        {
            for(int j = 0 ; j < 53 ; j++)
            {
                fscanf(f,"%c",&server_map[i][j]);
            }
            fscanf(f,"%c",&trash);
        }
        fclose(f);
    }

    void kill_player(int id)
    {
        players[id]={0,0,0,0,0,0,0,-1,id,NONE,(wchar_t)NULL};
        set_off_status(id);
        PIDS[id] = 0;
    }

    void beast_start()
    {
        number_of_beasts = 0;
        for(int i = 0 ; i < MAX_NUM_OF_BEASTS ; i++)
        {
            beast[i] = {0,0,0,0};
        }
    }

    void spaw_beast()
    {
        if(number_of_beasts < MAX_NUM_OF_BEASTS)
        {
            int x = rand()%50 + 2, y = rand()%24 + 2;
            while(get_symbol(x,y) != ' ')
            {
                x = rand()%50 + 2;
                y = rand()%24 + 2;
            }
            beast[number_of_beasts].beast_x = x;
            beast[number_of_beasts].beast_y = y;
            beast[number_of_beasts].on = 1;
            number_of_beasts++;
        }
    }

    void draw_beasts()
    {
        for(int i = 0 ; i < number_of_beasts ; i++)
        {
            wattron(board, COLOR_PAIR(RED));
            mvwprintw(board,beast[i].beast_y,beast[i].beast_x,"*");
            wattroff(board, COLOR_PAIR(RED));
        }
    }

    void beast_move_up(int i){ beast[i].beast_y--; }
    void beast_move_down(int i){ beast[i].beast_y++; }
    void beast_move_left(int i){ beast[i].beast_x--; }
    void beast_move_right(int i){ beast[i].beast_x++; }

    int beast_can_move_up(int i){ return(server_map[beast[i].beast_y - 1][beast[i].beast_x] == 'O' ? 0 : 1); }
    int beast_can_move_down(int i){ return(server_map[beast[i].beast_y + 1][beast[i].beast_x] == 'O' ? 0 : 1); }
    int beast_can_move_left(int i){ return(server_map[beast[i].beast_y][beast[i].beast_x - 1] == 'O' ? 0 : 1); }
    int beast_can_move_right(int i){ return(server_map[beast[i].beast_y][beast[i].beast_x + 1] == 'O' ? 0 : 1); }

    int beast_chase_player(int i, int id)
    {
        if(players[id].player_x > beast[i].beast_x && beast_can_move_right(i))
            beast_move_right(i);
        else if(players[id].player_x < beast[i].beast_x && beast_can_move_left(i))
            beast_move_left(i);
        else if(players[id].player_y < beast[i].beast_y && beast_can_move_up(i))
            beast_move_up(i);
        else if(players[id].player_y > beast[i].beast_y && beast_can_move_down(i))
            beast_move_down(i);

        if(beast[i].beast_x == players[id].player_x && beast[i].beast_y == players[id].player_y)
            return 0;
        
        return 1;
    }
    
    int visible_line(int i, int id)
    {
        if(server_map[beast[i].beast_y - (beast[i].beast_y - players[id].player_y)/2][beast[i].beast_x - (beast[i].beast_x - players[id].player_x)/2] == 'O')
            return 0;
        return 1;
    }

    int beast_see_player(int ib)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(abs(beast[ib].beast_x - players[i].player_x) < 2 && abs(beast[ib].beast_y - players[i].player_y) < 2)
                return i;
        }
        for(int i = 0 ; i < 4 ; i++)
        {
            if(abs(beast[ib].beast_x - players[i].player_x) == 2 && abs(beast[ib].beast_y - players[i].player_y) == 2)
                if(visible_line(ib,i))  return i;
        }
        return -1;
    }

    int get_beast_flag()
    {
        return beast_spawn_flag;
    }

    void remove_beast_flag()
    {
        beast_spawn_flag = 0;
    }

    int get_beast_num()
    {
        return number_of_beasts;
    }
};

int get_input(Server serv)
{
    return wgetch(serv.get_server_window());
}

    


    
