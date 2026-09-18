#ifndef EASYRPG_CORE_MOVIE_H
#define EASYRPG_CORE_MOVIE_H

#ifdef __cplusplus
extern "C"
{
#endif

    enum EasyRpgMovieState
    {
        EasyRpgMovieIdle = 0,
        EasyRpgMovieActive = 1,
        EasyRpgMovieDone = 2
    };

    int EasyRpgMovieStart(const char *path, int x, int y, int width, int height);
    int EasyRpgMoviePoll(void);
    void EasyRpgMovieAcknowledge(void);
    void EasyRpgMovieStop(void);

#ifdef __cplusplus
}
#endif

#endif
