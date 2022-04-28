#include "source.h"
#include "player.h"

pthread_mutex_t mutex;

void *update_map(void *arg)
{
	Player *p1 = (Player *)arg;
	Player_Stats p;
	int fo;
	sem_t *si;
	sem_t *so;
	char receive[4];
	
	if(p1->get_index() == 0)
	{
		strcpy(receive,"po1");
		si = sem_open(SEM_IN_1, 0);
		so = sem_open(SEM_OUT_1, 0);
	}
	else if(p1->get_index() == 1)
	{
		strcpy(receive,"po2");
		si = sem_open(SEM_IN_2, 0);
		so = sem_open(SEM_OUT_2, 0);
	}
	else if(p1->get_index() == 2)
	{
		strcpy(receive,"po3");
		si = sem_open(SEM_IN_3, 0);
		so = sem_open(SEM_OUT_3, 0);
	}
	else if(p1->get_index() == 3)
	{
		strcpy(receive,"po4");
		si = sem_open(SEM_IN_4, 0);
		so = sem_open(SEM_OUT_4, 0);
	}
	p1->set_sem(si,1);
	p1->set_sem(so,2);
	while(p.waiting != -1)
	{
		sem_wait(so);
		fo = open(receive,O_RDONLY);	
		read(fo, &p, sizeof(Player_Stats));		
		close(fo);							

		p1->move_view(p); 						
	}
	return (void *)NULL;
}

int main(int argc, char *argv[])
{
	//some declarations
	setlocale(LC_ALL, "");

	//initiating 
	srand(time(0));
	mutex = PTHREAD_MUTEX_INITIALIZER;

	//creating player
	int fd, fo;
	char send[3];
	char receive[4];
	int future_id = 0;
	comms usual = {0,getpid()};
	comms finito = {END_ME,getpid()};
	comms beninging = {SPAWN_ME,getpid()};
	sem_t *si;
	sem_t *so;
	pthread_t th;

	if(strcmp(argv[1], "p1") == 0)
	{
		strcpy(send,"p1");
		strcpy(receive,"po1");
		future_id = 0;
		si = sem_open(SEM_IN_1, 0);
		so = sem_open(SEM_OUT_1, 0);
	}
	else if(strcmp(argv[1], "p2") == 0)
	{
		strcpy(send,"p2");
		strcpy(receive,"po2");
		future_id = 1;
		si = sem_open(SEM_IN_2, 0);
		so = sem_open(SEM_OUT_2, 0);
	}
	else if(strcmp(argv[1], "p3") == 0)
	{
		strcpy(send,"p3");
		strcpy(receive,"po3");
		future_id = 2;
		si = sem_open(SEM_IN_3, 0);
		so = sem_open(SEM_OUT_3, 0);
	}
	else if(strcmp(argv[1], "p4") == 0)
	{
		strcpy(send,"p4");
		strcpy(receive,"po4");
		future_id = 3;
		si = sem_open(SEM_IN_4, 0);
		so = sem_open(SEM_OUT_4, 0);
	}

	sem_post(si);
	fd = open(send, O_WRONLY, 0);
	write(fd, &beninging, sizeof(comms));
	close(fd);

	sem_wait(so);

	Player_Stats p;

	fo = open(receive, O_RDONLY);
	read(fo, &p, sizeof(Player_Stats));
	close(fo);

	Player p1 = Player(p.player_x,p.player_y,p.number);
	p1.set_index(future_id);
    p1.move_view(p);

	p1.update_stats();

	pthread_create(&th,NULL,update_map,&p1);
	sem_post(si);
	usual.move = wgetch(p1.getwin());
	while(usual.move != 'q' && usual.move != 'Q')
	{
		sem_post(si);
		fd = open(send, O_WRONLY);
		write(fd, &usual, sizeof(comms));			
		close(fd);								
								
		usual.move = wgetch(p1.getwin());
	}

	return 0;
}