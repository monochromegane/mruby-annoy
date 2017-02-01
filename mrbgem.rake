MRuby::Gem::Specification.new('mruby-annoy') do |spec|
  spec.license = 'MIT'
  spec.authors = 'monochromegane'

  require 'open3'
  annoy_dir = "#{build_dir}/annoy"

  def run_command env, command
    STDOUT.sync = true
    puts "build: [exec] #{command}"
    Open3.popen2e(env, command) do |stdin, stdout, thread|
      print stdout.read
      fail "#{command} failed" if thread.value != 0
    end
  end

  FileUtils.mkdir_p build_dir

  unless File.exists? annoy_dir
    Dir.chdir(build_dir) do
      e = {}
      run_command e, 'git clone https://github.com/spotify/annoy.git'
    end
  end
  spec.cxx.include_paths << "#{annoy_dir}/src"
  spec.cxx.flags << "-std=c++11"
end
