# mruby-annoy   [![Build Status](https://travis-ci.org/monochromegane/mruby-annoy.svg?branch=master)](https://travis-ci.org/monochromegane/mruby-annoy)
AnnoyIndex class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'monochromegane/mruby-annoy'
end
```
## example
```ruby
p AnnoyIndex.hi
#=> "hi!!"
t = AnnoyIndex.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
