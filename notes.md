# clifunk [WIP]

Dynamic real-time CLI music visualizer, based on mpd stream    

## About

## Ideas

**Visualization:** 
* take in mpd stream
* do fourier transform
* divide the spectrum into three parts:
    low, mid, high
* gauge the levels of each separately, and calculate a separate visualization:
    * low: round, orb-like shape centered in the middle of the canvas, 
    pulsating to the rhythm (getting bigger at higher intensity and shrinking 
    slowly, etc)
    * middle: TBD, maybe sort of X-shape, hyperboloid something, also centered
    * high: possibly the coolest part - dots spread out all over the canvas, 
    maybe like a standing wave with a knot in the center, there's more of them
    when the highs go up in intensity

