#ifndef PACHI_GTP_H
#define PACHI_GTP_H

#define GTP_NO_REPLY (-2)

struct board;
struct engine;
struct time_info;

enum parse_code {
	P_OK,
	P_NOREPLY,
	P_DONE_OK,
	P_DONE_ERROR,
	P_ENGINE_RESET,
	P_UNKNOWN_COMMAND,
};

typedef struct
{
	char *cmd;
	char *next;
	int   id;
	int   replied;
} gtp_t;

#define next_tok(to_) \
	to_ = gtp->next; \
	gtp->next = gtp->next + strcspn(gtp->next, " \t\r\n"); \
	if (*gtp->next) { \
		*gtp->next = 0; gtp->next++; \
		gtp->next += strspn(gtp->next, " \t\r\n"); \
	}

enum parse_code gtp_parse(struct board *b, struct engine *e, struct time_info *ti, char *buf);
enum parse_code gtp_parse_full(struct board *b, struct engine *e, struct time_info *ti, char *buf, int id);
bool gtp_is_valid(struct engine *e, const char *cmd);
void gtp_final_score_str(struct board *board, struct engine *engine, char *reply, int len);
void gtp_reply(gtp_t *gtp, ...);
void gtp_reply_printf(gtp_t *gtp, const char *format, ...);
void gtp_error(gtp_t *gtp, ...);

#define is_gamestart(cmd) (!strcasecmp((cmd), "boardsize"))
#define is_reset(cmd) (is_gamestart(cmd) || !strcasecmp((cmd), "clear_board") || !strcasecmp((cmd), "kgs-rules"))
#define is_repeated(cmd) (strstr((cmd), "pachi-genmoves"))

/* Number of games played so far */
int  gtp_played_games();
void gtp_played_games_reset();

#endif
