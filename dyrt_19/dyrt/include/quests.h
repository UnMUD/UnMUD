#ifndef __QUESTS_H__ 
#define __QUESTS_H__ 


#define Q_EFOREST       0
#define Q_INVISWAND     1
#define Q_EXCALIBUR     2
#define Q_GRAIL         3
#define Q_FIERY_KING    4
#define Q_SABRE		5
#define Q_SPIKE		6
#define Q_BUST		7
#define Q_DRAGON	8
#define Q_ORB		9
#define Q_UNICORN	10
#define Q_FAFFNER	11
#define Q_ROD		12
#define Q_MJOLNIR	13
#define Q_CHICKEN	14
#define Q_SKI		15
#define Q_GUXX		16
#define Q_SUNDISC	17
#define Q_NARNIA	18
#define Q_ALL  ((1<<Q_EFOREST)|(1<<Q_INVISWAND)|(1<<Q_EXCALIBUR)|\
		(1<<Q_GRAIL)|(1<<Q_FIERY_KING)|(1<<Q_SABRE)|\
		(1<<Q_SPIKE)|(1<<Q_BUST)|(1<<Q_DRAGON)|(1<<Q_ORB)|\
		(1<<Q_UNICORN)|(1<<Q_FAFFNER)|(1<<Q_ROD)|\
		(1<<Q_MJOLNIR)|(1<<Q_CHICKEN)|(1<<Q_SKI)|(1<<Q_GUXX)|\
		(1<<Q_SUNDISC)|(1<<Q_NARNIA))

#define LAST_QUEST Q_NARNIA

#endif /* Add nothing past this line... */
