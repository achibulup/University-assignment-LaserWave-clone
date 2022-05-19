# Họ và tên: Trần Minh Sáng
# MSV: 21020026

Bài tập lớn INT2215_1: Trò chơi LaserWave


I: Hướng dẫn cài đặt

1. Clone repo về một folder nào đó thông qua lệnh
 git clone https://github.com/achibulup/University-assignment---project-LaserWave.git

2. Tải và cài đặt bộ dịch (compiler) GCC 7.3.0 MinGW(SEH) tại:
 Windows 32bit: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download
 Windows 64bit, MAC hoặc Linux: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download
 
-Lưu ý: Phải cài chính xác bộ dịch này.

 Sau đó, tìm thư mục Mingw được tạo ra và đưa đường dẫn của mingw/bin vào Path Environment Variables

3. Xây dựng chương trình từ mã nguồn
 Mở Git Bash, đưa đến thư mục repo đã được clone về, nhập lệnh mingw32-make
 Khi chạy xong, chương trình LaswerWave.exe sẽ được tạo ra ở thư mục bin/Debug

4. Chạy trương trình
 Vào thư mục bin/Debug, chạy chương trình LaserWave.exe
 Nếu không chạy được thì hãy chạy file run.sh (bằng Git Bash)
 
 
II: Mô tả chung về trò chơi

Đây là trò chơi né tránh và bắn các thiên thạch theo nhịp điệu. Mỗi khi tiếng trống kick phát lên thì nhân vật chính sẽ nhảy xa ra khỏi con trỏ, còn khi tiếng trống snare phát lên thì nhân vật sẽ bắn về hướng con trỏ. Nhiệm vụ của người chơi là dùng con trỏ điều chỉnh hướng di chuyển và bắn của nhân vật theo nhịp điệu và sống sót càng lâu càng tốt.

III: Mô tả các chức năng


- Khi mở ra sẽ có splashscreen xuất hiện trong vài giây, có thể click chuột trái rút ngắn thời gian này.
- Sau splashscreen sẽ xuất hiện menu. Ở đây người chơi nhấn bất kỳ đâu để chơi hoặc nhấn nút exit ở góc màn hình để thoát.
- Khi chơi, bạn sẽ điều khiển hình thoi để tránh và bắn chướng ngại vật.
- Có âm thanh khi bắn, nhảy và khi va chạm.
- Có đồng hồ phía trên màn hình để theo dõi thời gian sống sót.
- Có thanh máu ở góc trái trên màn mình.
- Khi va chạm, bạn sẽ được miễn sát thương va chạm trong một thời gian ngắn.
- Có thể nhấn P hoặc Esc để tạm dừng. Ở đây có nút tiếp tục, chơi lại hoặc về menu.

IV: Các kỹ thuật lập trình được sử dụng
- Các yếu tố cơ bản đã được dạy trên lớp: mảng, hàm, con trỏ, struct, class
- Nâng cao: template, hướng đối tượng, thư viện đồ họa, module hóa, một số nguyên tắc trong lập trình, một số kỹ thuật khác.

V: Kết luận
- Game này còn nhiều thiếu sót do vấn đề thời gian: chưa có bảng điểm, lưu tiến trình, không có nhiều loại địch, không có boss, không có level, tùy chỉnh... nhưng
- Trong khi làm game đã học được nhều thứ: biết cấu trúc, cách sử dụng của một thư viện đơn giản, hiểu cơ bản cách tự tạo thư viện cho mình; Đã vận dụng được nhiều kiến thức về lập trình đã được học, nhất là các kiến thức nâng cao; Rèn luyện, hiểu thêm các nguyên tắc trong lập trình; Hiểu cơ bản cấu trúc của một trò chơi thông thường.
- Phương hướng phát triển: thêm các tính năng cho trò chơi (các tính năng còn thiếu đã nói ở trên); học thêm các thư viện, framework, học hỏi thêm các kỹ thuật lập trình.
