# mruby-annoy   [![Build Status](https://travis-ci.org/monochromegane/mruby-annoy.svg?branch=master)](https://travis-ci.org/monochromegane/mruby-annoy)

AnnoyIndex class for mruby.

[Annoy](https://github.com/spotify/annoy) is a C++ library to search for points in space that are close to a given query point. This mrbgem provides mruby bindings for the library.

## Ruby code example

```ruby
f = 40
t = AnnoyIndex.new(f)  # Length of item vector that will be indexed
1000.times do |i|
  v = f.times.map{rand}
  t.add_item(i, v)
end

t.build(10) # 10 trees
t.save('test.ann')

# ...

u = AnnoyIndex.new(f)
u.load('test.ann') # super fast, will just mmap the file
puts u.get_nns_by_item(0, 1000) # will find the 1000 nearest neighbors
```

API are compatible with [Annoy Full Python API](https://github.com/spotify/annoy#full-python-api)

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'monochromegane/mruby-annoy'
end
```

## License

[MIT](https://github.com/monochromegane/mruby-annoy/blob/master/LICENSE)

## Author

[monochromegane](https://github.com/monochromegane)
