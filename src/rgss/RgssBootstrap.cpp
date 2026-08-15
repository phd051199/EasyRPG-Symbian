#include "ruby.h"

/*
 * Ruby 1.8 (RGSS1) -> Ruby 3.0 compatibility bootstrap, evaluated right
 * after the C bindings are in place. Renderer classes are native bindings;
 * Font is the remaining temporary renderer shim until text rendering lands.
 */

namespace {

const char KBootstrap[] =
    "Object.const_set(:Fixnum, Integer) unless Object.const_defined?(:Fixnum)\n"
    "Object.const_set(:Bignum, Integer) unless Object.const_defined?(:Bignum)\n"
    "\n"
    "module Kernel\n"
    "  def rgss_main\n"
    "    yield if block_given?\n"
    "  end\n"
    "  def rgss_stop\n"
    "    loop { Graphics.update }\n"
    "  end\n"
    "end\n"
    "\n"
    "# Windows-only RGSS compatibility surface. Calls are inert on Symbian:\n"
    "# keep load-time feature probes working without executing arbitrary DLLs.\n"
    "class Win32API\n"
    "  def initialize(*a); end\n"
    "  def call(*a); 0; end\n"
    "  alias Call call\n"
    "end\n"
    "\n"
    "module RgssInternal\n"
    "  def self.fix_encoding(obj)\n"
    "    case obj\n"
    "    when String\n"
    "      obj.force_encoding(Encoding::UTF_8) if obj.encoding == Encoding::ASCII_8BIT\n"
    "    when Array\n"
    "      obj.each { |o| fix_encoding(o) }\n"
    "    when Hash\n"
    "      obj.each { |k, v| fix_encoding(k); fix_encoding(v) }\n"
    "    else\n"
    "      if obj.respond_to?(:instance_variables, true)\n"
    "        obj.instance_variables.each do |iv|\n"
    "          fix_encoding(obj.instance_variable_get(iv))\n"
    "        end\n"
    "      end\n"
    "    end\n"
    "    obj\n"
    "  end\n"
    "end\n"
    "\n"
    "# mkxp-z extension module expected by games built for it\n"
    "# (MKXP_Compatibility / Essentials-class scripts).\n"
    "module System\n"
    "  def self.set_window_title(title); end\n"
    "  def self.game_title; 'EasyRPG'; end\n"
    "  def self.mouse_in_window; true; end\n"
    "  def self.mouse_in_window=(v); end\n"
    "  def self.delta; 0; end\n"
    "  def self.data_directory; '.'; end\n"
    "end\n"
    "\n"
    "# --- Font is the last renderer bring-up shim. ---\n"
    "class Font\n"
    "  @default_name = ['']\n"
    "  @default_size = 22\n"
    "  @default_bold = false\n"
    "  @default_italic = false\n"
    "  class << self\n"
    "    attr_accessor :default_name, :default_size, :default_bold, :default_italic\n"
    "  end\n"
    "  attr_accessor :name, :size, :bold, :italic, :color\n"
    "  def initialize(name = nil, size = nil)\n"
    "    @name = name || Font.default_name\n"
    "    @size = size || Font.default_size\n"
    "    @bold = Font.default_bold\n"
    "    @italic = Font.default_italic\n"
    "    @color = Color.new(255, 255, 255, 255)\n"
    "  end\n"
    "end\n"
    "\n"
    "$DEBUG = false\n"
    "$TEST = false\n"
    "$BTEST = false\n";

const char* KFileBootstrap = R"RGSS(
module RgssInternal
  def self.binary_string(value = '')
    value.dup.force_encoding(Encoding::BINARY)
  end

  def self.path_string(path)
    path = path.to_path if path.respond_to?(:to_path)
    path.to_s
  end

  def self.relative_path?(path)
    path = path_string(path)
    return false if path.empty? || path.start_with?('/', '\\')
    path !~ /\A[A-Za-z]:[\\\/]/
  end

  def self.write_mode?(mode)
    if mode.is_a?(Integer)
      return (mode & (File::WRONLY | File::RDWR | File::CREAT | File::TRUNC | File::APPEND)) != 0
    end
    text = mode.to_s
    text.include?('w') || text.include?('a') || text.include?('+') || text.include?('x')
  end

  def self.append_mode?(mode)
    mode.is_a?(Integer) ? ((mode & File::APPEND) != 0) : mode.to_s.include?('a')
  end

  def self.truncate_mode?(mode)
    mode.is_a?(Integer) ? ((mode & File::TRUNC) != 0) : mode.to_s.include?('w')
  end

  class VirtualFile
    attr_reader :path
    attr_accessor :sync

    def initialize(path, mode, bytes)
      @path = RgssInternal.path_string(path)
      @mode = mode
      @writable = RgssInternal.write_mode?(mode)
      @append = RgssInternal.append_mode?(mode)
      @data = RgssInternal.binary_string(bytes || '')
      @pos = @append ? @data.bytesize : 0
      @closed = false
      @dirty = false
      @sync = false
    end

    def inspect
      "#<#{self.class}:#{@path}#{closed? ? ' (closed)' : ''}>"
    end

    def to_path; @path; end
    def closed?; @closed; end
    def eof?; check_open; @pos >= @data.bytesize; end
    alias eof eof?
    def pos; check_open; @pos; end
    alias tell pos
    def size; check_open; @data.bytesize; end
    def binmode; check_open; self; end
    def binmode?; true; end
    def external_encoding; Encoding::BINARY; end
    def internal_encoding; nil; end

    def pos=(value)
      seek(Integer(value), IO::SEEK_SET)
      value
    end

    def seek(amount, whence = IO::SEEK_SET)
      check_open
      amount = Integer(amount)
      base = case whence
             when IO::SEEK_SET then 0
             when IO::SEEK_CUR then @pos
             when IO::SEEK_END then @data.bytesize
             else raise Errno::EINVAL, @path
             end
      target = base + amount
      raise Errno::EINVAL, @path if target < 0
      @pos = target
      0
    end

    def rewind
      seek(0, IO::SEEK_SET)
      0
    end

    def read(length = nil, outbuf = nil)
      check_open
      if length.nil?
        result = @data.byteslice(@pos, @data.bytesize - @pos) || RgssInternal.binary_string
        @pos = @data.bytesize
      else
        length = Integer(length)
        raise ArgumentError, 'negative length' if length < 0
        if length == 0
          result = RgssInternal.binary_string
        elsif @pos >= @data.bytesize
          result = nil
        else
          result = @data.byteslice(@pos, length)
          @pos += result.bytesize
        end
      end
      if outbuf
        outbuf.replace(result || RgssInternal.binary_string)
        return nil if result.nil?
        return outbuf
      end
      result
    end

    def readpartial(length, outbuf = nil)
      result = read(length, outbuf)
      raise EOFError if result.nil?
      result
    end
    alias sysread readpartial

    def getbyte
      check_open
      return nil if @pos >= @data.bytesize
      value = @data.getbyte(@pos)
      @pos += 1
      value
    end

    def getc
      value = getbyte
      value.nil? ? nil : value.chr(Encoding::BINARY)
    end

    def gets(separator = $/, limit = nil, chomp: false)
      check_open
      return nil if eof?
      if separator.nil?
        line = read
      else
        separator = separator.to_s
        raise ArgumentError, 'empty separator is not supported' if separator.empty?
        finish = @data.index(separator, @pos)
        finish = finish ? finish + separator.bytesize : @data.bytesize
        finish = [finish, @pos + Integer(limit)].min if limit
        line = @data.byteslice(@pos, finish - @pos)
        @pos = finish
      end
      line = line.chomp(separator || $/) if chomp
      line
    end

    def readline(*args, **opts)
      line = gets(*args, **opts)
      raise EOFError if line.nil?
      line
    end

    def each_line(*args, **opts)
      return enum_for(__method__, *args, **opts) unless block_given?
      while (line = gets(*args, **opts))
        yield line
      end
      self
    end
    alias each each_line

    def readlines(*args, **opts)
      each_line(*args, **opts).to_a
    end

    def each_byte
      return enum_for(__method__) unless block_given?
      while (value = getbyte)
        yield value
      end
      self
    end

    def write(value)
      check_open
      raise IOError, 'not opened for writing' unless @writable
      bytes = RgssInternal.binary_string(value.to_s)
      @pos = @data.bytesize if @append
      if @pos > @data.bytesize
        @data << "\0" * (@pos - @data.bytesize)
      end
      @data[@pos, bytes.bytesize] = bytes
      @pos += bytes.bytesize
      @dirty = true
      flush if @sync
      bytes.bytesize
    end
    alias syswrite write

    def <<(value); write(value); self; end
    def print(*values); values.each { |value| write(value.to_s) }; nil; end
    def printf(format, *values); write(format % values); nil; end
    def puts(*values)
      values = [''] if values.empty?
      values.each do |value|
        text = value.to_s
        write(text)
        write($/) unless text.end_with?($/)
      end
      nil
    end

    def truncate(length)
      check_open
      raise IOError, 'not opened for writing' unless @writable
      length = Integer(length)
      raise Errno::EINVAL, @path if length < 0
      if length < @data.bytesize
        @data = @data.byteslice(0, length)
      elsif length > @data.bytesize
        @data << "\0" * (length - @data.bytesize)
      end
      @pos = length if @pos > length
      @dirty = true
      length
    end

    def flush
      check_open
      commit
      self
    end

    def fsync
      flush
      0
    end

    def close
      return nil if @closed
      commit
      @closed = true
      nil
    end

    def close_read; close; end
    def close_write; close; end

    private

    def check_open
      raise IOError, 'closed stream' if @closed
    end

    def commit
      return unless @writable && @dirty
      RgssInternal.__vfs_write(@path, @data)
      @dirty = false
    end
  end
end

class << File
  alias __rgss_native_open open
  alias __rgss_native_read read
  alias __rgss_native_binread binread
  alias __rgss_native_readlines readlines
  alias __rgss_native_foreach foreach
  alias __rgss_native_write write
  alias __rgss_native_binwrite binwrite if method_defined?(:binwrite)
  alias __rgss_native_delete delete
  alias __rgss_native_rename rename
  alias __rgss_native_truncate truncate
  alias __rgss_native_exist exist?
  alias __rgss_native_file file?
  alias __rgss_native_size size

  def open(path, *args, **options)
    mode = args.empty? ? 'r' : args[0]
    path_string = RgssInternal.path_string(path)
    virtual = false
    bytes = nil

    if RgssInternal.write_mode?(mode) && RgssInternal.relative_path?(path_string)
      virtual = true
      if !RgssInternal.truncate_mode?(mode) && RgssInternal.__vfs_exist?(path_string)
        bytes = RgssInternal.__vfs_read(path_string)
      else
        bytes = RgssInternal.binary_string
      end
    elsif RgssInternal.__vfs_exist?(path_string)
      virtual = true
      bytes = RgssInternal.__vfs_read(path_string)
    end

    unless virtual
      return __rgss_native_open(path, *args, **options) unless block_given?
      return __rgss_native_open(path, *args, **options) { |file| yield file }
    end

    file = RgssInternal::VirtualFile.new(path_string, mode, bytes)
    return file unless block_given?
    begin
      yield file
    ensure
      file.close unless file.closed?
    end
  end

  def read(path, *args, **options)
    return __rgss_native_read(path, *args, **options) unless RgssInternal.__vfs_exist?(RgssInternal.path_string(path))
    length = args[0]
    offset = args[1]
    open(path, 'rb') do |file|
      file.seek(offset, IO::SEEK_SET) if offset
      length ? file.read(length) : file.read
    end
  end

  def binread(path, *args)
    return __rgss_native_binread(path, *args) unless RgssInternal.__vfs_exist?(RgssInternal.path_string(path))
    open(path, 'rb') do |file|
      length = args[0]
      offset = args[1]
      file.seek(offset, IO::SEEK_SET) if offset
      length ? file.read(length) : file.read
    end
  end

  def write(path, string, *args, **options)
    path_string = RgssInternal.path_string(path)
    return __rgss_native_write(path, string, *args, **options) unless RgssInternal.relative_path?(path_string)
    offset = args[0]
    mode = offset ? 'r+b' : 'wb'
    open(path_string, mode) do |file|
      file.seek(offset, IO::SEEK_SET) if offset
      file.write(string)
    end
  end

  def binwrite(path, string, *args)
    path_string = RgssInternal.path_string(path)
    if !RgssInternal.relative_path?(path_string) && respond_to?(:__rgss_native_binwrite)
      return __rgss_native_binwrite(path, string, *args)
    end
    write(path_string, string, *args)
  end

  def delete(*paths)
    count = 0
    paths.each do |path|
      path_string = RgssInternal.path_string(path)
      if RgssInternal.relative_path?(path_string) && RgssInternal.__vfs_delete(path_string)
        count += 1
      else
        count += __rgss_native_delete(path)
      end
    end
    count
  end
  alias unlink delete

  def rename(old_path, new_path)
    old_string = RgssInternal.path_string(old_path)
    new_string = RgssInternal.path_string(new_path)
    if RgssInternal.relative_path?(old_string) && RgssInternal.relative_path?(new_string) &&
       RgssInternal.__vfs_rename(old_string, new_string)
      return 0
    end
    __rgss_native_rename(old_path, new_path)
  end

  def truncate(path, length)
    path_string = RgssInternal.path_string(path)
    unless RgssInternal.relative_path?(path_string) && RgssInternal.__vfs_exist?(path_string)
      return __rgss_native_truncate(path, length)
    end
    open(path_string, 'r+b') { |file| file.truncate(length) }
    0
  end

  def readlines(path, *args, **options)
    return __rgss_native_readlines(path, *args, **options) unless RgssInternal.__vfs_exist?(RgssInternal.path_string(path))
    open(path, 'rb') { |file| file.readlines(*args, **options) }
  end

  def foreach(path, *args, **options)
    return enum_for(__method__, path, *args, **options) unless block_given?
    unless RgssInternal.__vfs_exist?(RgssInternal.path_string(path))
      return __rgss_native_foreach(path, *args, **options) { |line| yield line }
    end
    open(path, 'rb') { |file| file.each_line(*args, **options) { |line| yield line } }
  end

  def exist?(path)
    RgssInternal.__vfs_exist?(RgssInternal.path_string(path)) || __rgss_native_exist(path)
  end
  alias exists? exist?

  def file?(path)
    RgssInternal.__vfs_exist?(RgssInternal.path_string(path)) || __rgss_native_file(path)
  end

  def size(path)
    return __rgss_native_size(path) unless RgssInternal.__vfs_exist?(RgssInternal.path_string(path))
    RgssInternal.__vfs_read(RgssInternal.path_string(path)).bytesize
  end
end

module FileTest
  class << self
    def exist?(path); File.exist?(path); end
    alias exists? exist?
    def file?(path); File.file?(path); end
    def size(path); File.size(path); end
    def size?(path); size(path).zero? ? nil : size(path); rescue nil; end
    def zero?(path); size(path).zero?; rescue false; end
  end
end
)RGSS";

} // namespace

int RgssEvaluateBootstrap()
{
    int state = 0;
    rb_eval_string_protect(KBootstrap, &state);
    if (state)
        return state;
    rb_eval_string_protect(KFileBootstrap, &state);
    return state;
}
