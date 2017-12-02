# dpmswatch

Run scripts when DPMS status changes

## Usage

Let's say you have a script to change the backlight on your keyboard. It makes sense to turn the keylights off when the screen goes off, and to turn them back on when the screen comes back. This can be accomplished easily with:

```bash
dpmswatch -n "keylight up" -f "keylight off"
```

This will run `keylight up` when the screen comes o**n** and `keylight off` when the screen goes of**f**.

`dpmswatch` should use barely any resources, so it's cool to set and forget. `htop` reports a steady 0.0% CPU and 0.0% memory usage.
