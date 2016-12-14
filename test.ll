declare void @avrlit_initialize();

declare void @avrlit_report_result_u8(i8* %name, i8 %result);
declare void @avrlit_report_result_u16(i8* %name, i16 %result);
declare void @avrlit_report_result_u32(i8* %name, i32 %result);
declare void @avrlit_report_result_u64(i8* %name, i64 %result);
declare void @avrlit_report_result_i8(i8* %name, i8 %result);
declare void @avrlit_report_result_i16(i8* %name, i16 %result);
declare void @avrlit_report_result_i32(i8* %name, i32 %result);
declare void @avrlit_report_result_i64(i8* %name, i64 %result);

@result_name = private unnamed_addr constant [7 x i8] c"result\00"

define void @main() {
  call void @avrlit_initialize()

  %cast210 = getelementptr [7 x i8], [7 x i8]* @result_name, i64 0, i64 0
  call void @avrlit_report_result_u8(i8* %cast210, i8 62)
  ret void
}
