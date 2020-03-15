# List of Bugs

## UI
### Step Sequence
* when `follow_cursor` is NOT on, the last step remains on (unless it is the last step on the physical page)
* when `follow_cursor` is NOT on, and `show_last_step` is pressed and unpressed and we are on the same page as the last step, the animation for the last step doesn't turn off.
* when `follow_cursor` IS on, and we are on the last page and press/unpress `show_last_step` the animation only turns off when the cursor has moved to the new page.
* intermittent-- when `follow_cursor` is NOT on, and switching between pages quickly, sometimes the cursor gets "stuck" on in the wrong page.
