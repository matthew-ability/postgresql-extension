# Hamming Distance for Postgresql

Hamming Distance Module for String Comparison in Postgresql

## Installation

install needed build dependencies: On ubuntu for instance: `postgresql-server-dev-12 gcc make`

```
   make
   make install
```

## Activation

```
   sudo -u postgres psql <db>
   create extension hammingdistance;
```



## Usage
```
SELECT hammingdistance('ABC', 'ABD');  -- returns 1
```


