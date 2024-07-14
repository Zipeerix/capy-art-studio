## Project files (.capy)

### 0x00 - 0x04: Magic string

0x00 - 'C'

0x01 - 'A'

0x02 - 'P'

0x03 - 'A'

0x04 - 0x00

### 0x05 - 0x08: Miniature size

Exact size of miniature image that is in the next segment of this file. Represented by 32-bit (4-byte) little-endian
integer.

### 0x08 - 0x??: Miniature data

Miniature of the project in format that can be loaded in QPixmap (preferably PNG).

### (after miniature data) 0x00 - 0x??: TBD

TBD

## Palette files (.json)

Example of Palette file:

```json
{
  "name": "MyPalette",
  "colors": [
    {
      "hint": "This is my color1",
      "color": {
        "r": 255,
        "g": 128,
        "b": 64,
        "alpha": 44
      }
    },
    {
      "hint": "This is my color2",
      "color": {
        "r": 255,
        "g": 0,
        "b": 0,
        "alpha": 255
      }
    }
  ]
}
```

Both ```name``` and ```colors``` fields are mandatory even if there are no colors present in the list.
Within ```colors``` the field ```hint``` is optional while ```color``` is not.