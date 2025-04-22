# jaudio

## build

```bash
make
```

## synth

create synthetic wav files

```bash
./test_synth.out
```

and play them with

```bash
ffplay sin.wav
```

## plotting

plot wav files using gnuplot with
```bash
./wav_plot.out file.wav && gnuplot -pc file.wav.gp
```

or using raylib
```bash
./wav_view.out file.wav
```

## TODO
- `wav_view`:
    - multiple channel support
    - playback using alsa
    - record using alsa
    - implement spectrogram
- reimplement `wav_view.c` using x11 or glfw?
