
#include "source.h"
#include "server.h"

Server *server;
pthread_mutex_t mutex;
pthread_mutex_t beast_mutex;

//open semaphores
sem_t* si1 = sem_open(SEM_IN_1, O_CREAT, 0666, 0); 
sem_t* si2 = sem_open(SEM_IN_2, O_CREAT, 0666, 0); 
sem_t* si3 = sem_open(SEM_IN_3, O_CREAT, 0666, 0); 
sem_t* si4 = sem_open(SEM_IN_4, O_CREAT, 0666, 0); 
sem_t* so1 = sem_open(SEM_OUT_1, O_CREAT, 0666, 0); 
sem_t* so2 = sem_open(SEM_OUT_2, O_CREAT, 0666, 0); 
sem_t* so3 = sem_open(SEM_OUT_3, O_CREAT, 0666, 0); 
sem_t* so4 = sem_open(SEM_OUT_4, O_CREAT, 0666, 0); 
sem_t *serv1 = sem_open(SEM_SERV_1, O_CREAT, 0666, 0);
sem_t *serv2 = sem_open(SEM_SERV_2, O_CREAT, 0666, 0);
sem_t *serv3 = sem_open(SEM_SERV_3, O_CREAT, 0666, 0);
sem_t *serv4 = sem_open(SEM_SERV_4, O_CREAT, 0666, 0);
sem_t *beast = sem_open(BEAST, O_CREAT, 0666, 0);

int set_player_view(int id)
    {
		sem_t *so;
		int x,y;
		char write_to[4];
		for(int i = 0 ; i < 5 ; i++)
		{
			y = server->get_y(id) - 2 + i;
			x = server->get_x(id) - 2;
			for(int j = 0 ; j < 5 ; j++)
			{
				server->players[id].map[i * 5 + j] = mvwinch(server->get_server_window(),y,x);
				x++;
			}
		}
		server->players[id].round = server->get_round();
		int fo;
		switch(id)
		{
			case 0:
			strcpy(write_to,"po1");
			so = so1;
			break;

			case 1:
			strcpy(write_to,"po2");
			so = so2;
			break;

			case 2:
			strcpy(write_to,"po3");
			so = so3;
			break;

			case 3:
			strcpy(write_to,"po4");
			so = so4;
			break;
		}
		if(server->get_status(id) && kill(server->get_pids(id),0) != 0)
		{
			server->kill_player(id);
			return 0;
		}
		sem_post(so);
		fo = open(write_to, O_WRONLY);
		write(fo, &(server->players[id]), sizeof(Player_Stats));
        wrefresh(server->get_server_window());
		close(fo);
		return 0;
    }

void *read_fifo(void *arg)
    {
        int fd;
		static comms moves[4] = {0,0,0,0,0,0,0,0};
        int i = *((int *)arg);
		char to_open[3];
		sem_t *si;
		sem_t *serv;
        switch (i)
        {
            case 0:
            strcpy(to_open,"p1");
			si = si1;
			serv = serv1;
            break;

            case 1:
            strcpy(to_open,"p2");
			si = si2;
			serv = serv2;
            break;

            case 2:
            strcpy(to_open,"p3");
			si = si3;
			serv = serv3;
            break;

            case 3:
			strcpy(to_open,"p4");
			si = si4;
			serv = serv4;
            break;
        }
		while(1)
		{
				
			sem_wait(si);

			if(server->players[i].waiting == -1)	break;
			
			fd = open(to_open, O_RDONLY);
			read(fd, &moves[i], sizeof(comms));
			close(fd);

			if(moves[i].move == SPAWN_ME)	server->set_player_PID(i,moves[i].process_id);

			pthread_mutex_lock(&mutex);
			server->move_player(i,moves[i].move);
			pthread_mutex_unlock(&mutex);

			sem_wait(serv);
		}

		return (void *)NULL;
    }

	void *handle_beast(void *arg)
	{
		server->spaw_beast();
		server->remove_beast_flag();
		int i = *((int*)arg);
		int random;
		while(1)
		{
			pthread_mutex_lock(&beast_mutex);
			if(server->beast_see_player(i) != -1)
			{
				int killed;
				int see = server->beast_see_player(i);
				while(1)
				{
					killed = server->beast_chase_player(i,see);
					if(killed == 1)	break;
					see = server->beast_see_player(i);
					if(see == -1)	break;
				}
				if(killed)	server->kill_player_beast(see);
			}
				

			random = rand()%4;
			switch(random)
			{
				case 0:
				if(server->beast_can_move_up(i))	server->beast_move_up(i);
				break;

				case 1:
				if(server->beast_can_move_down(i))	server->beast_move_down(i);
				break;

				case 2:
				if(server->beast_can_move_left(i))	server->beast_move_left(i);
				break;

				case 3:
				if(server->beast_can_move_right(i))	server->beast_move_right(i);
				break;
			}
			pthread_mutex_unlock(&beast_mutex);
			usleep(ROUND_TIME / 2 * SECOND);
		}
	}

    int handle_communication()
    {
        pthread_t th[4]; 
		pthread_t b[10]; 
        int args[4] = {0,1,2,3};
		int args_b[10] = {0,1,2,3,4,5,6,7,8,9};
		int f;
		for(int i = 0 ; i < 4 ; i++)
		{
			pthread_create(&(th[i]),NULL,read_fifo,&args[i]);
		}
		while(1)
		{	
			if(server->get_beast_flag())
			{
				pthread_create(&(b[server->get_beast_num()]),NULL,handle_beast,&args_b[server->get_beast_num()]);
				usleep(0.1 * SECOND);
			}	
			sem_post(beast);			
			
			if(server->get_status(0) == 1)
			{
				if(set_player_view(0))
				{
					sem_post(si1);
					pthread_join(th[0],NULL);
					server->players[0].waiting = 0;
					pthread_create(&(th[0]),NULL,read_fifo,&args[0]);
				}
				sem_post(serv1);
			}
			if(server->get_status(1) == 1)
			{
				if(set_player_view(1))
				{
					sem_post(si2);
					pthread_join(th[1],NULL);
					server->players[1].waiting = 0;
					pthread_create(&(th[1]),NULL,read_fifo,&args[1]);
				}
				sem_post(serv2);
			}
			if(server->get_status(2) == 1)
			{
				if(set_player_view(2))
				{
					sem_post(si3);
					pthread_join(th[2],NULL);
					server->players[2].waiting = 0;
					pthread_create(&(th[2]),NULL,read_fifo,&args[2]);
				}
				sem_post(serv3);
			}
			if(server->get_status(3) == 1)
			{
				if(set_player_view(3))
				{
					sem_post(si4);
					pthread_join(th[3],NULL);
					server->players[3].waiting = 0;
					pthread_create(&(th[3]),NULL,read_fifo,&args[3]);
				}
				sem_post(serv4);
			}
			server->draw_game_v2();
			usleep(SECOND * ROUND_TIME);
			server->server_next_round();
		}
		for(int i = 0 ; i < 4 ; i++)
		{
			pthread_join(th[i],NULL);
		}
		
		for(int i = 0 ; i < server->get_beast_num() ; i++)
		{
			pthread_join(b[i],NULL);
		}

        return 0;
    };

int main() 
{
	//TODO

	// -bestie --> smierc, utrata pieniedzy
	// problem przy opuszczaniu gry



	//some declarations
	setlocale(LC_ALL, "");

	//initiating 
	srand(time(0));
	server = new Server();
	usleep(0.1 * SECOND);

	server->set_sems(si1,0); 
	server->set_sems(si2,1); 
	server->set_sems(si3,2); 
	server->set_sems(si4,3); 
	server->set_sems(so1,4); 
	server->set_sems(so2,5); 
	server->set_sems(so3,6); 
	server->set_sems(so4,7); 
	server->set_sems(serv1,8); 
	server->set_sems(serv2,9); 
	server->set_sems(serv3,10);
	server->set_sems(serv4,11);

	server->load_map();
	usleep(0.1 * SECOND);

	//initiate mutex
    mutex = PTHREAD_MUTEX_INITIALIZER;
	beast_mutex = PTHREAD_MUTEX_INITIALIZER;

	//make fifos
	mknod("p1", S_IFIFO|0666, 0);
	mknod("po1", S_IFIFO|0666, 0);

	mknod("p2", S_IFIFO|0666, 0);
	mknod("po2", S_IFIFO|0666, 0);

	mknod("p3", S_IFIFO|0666, 0);
	mknod("po3", S_IFIFO|0666, 0);

	mknod("p4", S_IFIFO|0666, 0);
	mknod("po4", S_IFIFO|0666, 0);

	//communication between processes
	handle_communication();


	return 0;
}

