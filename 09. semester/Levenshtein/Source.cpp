//
//data_element compute_diagonal()
//{
//	const int diag_count = 6;
//	int diag_len = 1;
//	int step = 1;
//
//	size_t diag_size = size1;
//	diag1 = new data_element[diag_size + 1];
//	diag2 = new data_element[diag_size + 1];
//	diag3 = new data_element[diag_size + 1];
//
//	// total number of diagonals
//	int total_diags = size1 + size2 + 1;
//
//	// init
//	diag2[0] = 0;
//	diag1[0] = 1;
//	diag1[1] = 1;
//
//	for (int d = 2; d < total_diags; ++d)
//	{	// process each diagonal
//
//		diag3[0] = diag_len;	// ??
//		for (int i = 1; i < diag_len; ++i)
//		{	// process one diagonal
//			int i1 = i - 1;
//			char cost = (str1[step - 1 - i] == str2[i1]) ? 0 : 1;
//
//			data_element upper = diag1[i1];
//			data_element left = diag1[i];
//			data_element upperleft = diag2[i1];
//
//			diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
//		}
//		diag3[diag_len] = diag_len;	// ??
//
//		// rotate diagonals
//		auto tmp = diag2;
//		diag2 = diag1;
//		diag1 = diag3;
//		diag3 = tmp;
//	}
//
//	/*for (int step = 2; step <= diag_size; ++step)
//	{
//	for (int i = 1; i < step; ++i)
//	{
//	int i1 = i - 1;
//	char cost = (str1[step - 1 - i] == str2[i1]) ? 0 : 1;
//
//	data_element upper = diag1[i1];
//	data_element left = diag1[i];
//	data_element upperleft = diag2[i1];
//
//	diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
//	}
//
//	diag3[0] = step;
//	diag3[step] = step;
//
//	for (int i = 0; i <= step; ++i)
//	std::cout << diag3[i] << " ";
//	std::cout << std::endl;
//
//	auto tmp = diag2;
//	diag2 = diag1;
//	diag1 = diag3;
//	diag3 = tmp;
//	}*/
//
//	return diag1[1];
//}
//
//data_element compute_chunks()
//{
//	int chunk1 = 2;
//	int chunk2 = 2;
//
//	left = new data_element[size1 + 1];
//	for (int i = 0; i <= size1; ++i)
//		left[i] = i;
//	top = new data_element[size2 + 1];
//	for (int i = 0; i <= size2; ++i)
//		top[i] = i;
//
//
//
//
//	return 0;
//}
//data_element compute_matrix2()
//{
//	size_t diag_size = size1;
//	diag1 = new data_element[diag_size + 1];
//	diag2 = new data_element[diag_size + 1];
//	diag3 = new data_element[diag_size + 1];
//
//	// init
//	diag2[0] = 0;
//	diag1[0] = 1;
//	diag1[1] = 1;
//
//	int diag_len;
//
//	// upper-left matrix triangle - diagonal size is growing
//	for (diag_len = 1; diag_len < size2; ++diag_len)
//	{
//		int j = 1;
//		diag3[diag_len + 1] = diag_len;
//		for (int i = diag_len; i > 0; --i, ++j)
//		{
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//			diag3[i] = smaller<data_element>(diag1[i - 1] + 1, diag1[i] + 1, diag2[i - 1] + cost);
//		}
//		diag3[0] = diag_len;
//
//		print_diagonal(diag3, diag_len);
//
//		rotate(diag2, diag1, diag3);
//	}
//
//	// middle of the matrix - diagonal is constant
//	diag_len = size2;
//	for (int x = size2; x < size1; ++x)
//	{
//		int i = x;
//		for (int j = 1; j <= diag_len; ++j, --i)
//		{
//			data_element upper = get(i - 1, j);
//			data_element left = get(i, j - 1);
//			data_element upperleft = get(i - 1, j - 1);
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//
//			set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
//		}
//	}
//	// bottom-right matrix triangle - diagonal size is 
//	for (diag_len = size2 - 1; diag_len >= 0; --diag_len)
//	{
//		int i = size1;
//		for (int j = size2 - diag_len; j <= size2; ++j, --i)
//		{
//			data_element upper = get(i - 1, j);
//			data_element left = get(i, j - 1);
//			data_element upperleft = get(i - 1, j - 1);
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//
//			set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
//		}
//	}
//
//	return get(size1, size2);
//}
//
//data_element compute_two_rows_skew()
//{
//	if (size1 == 0) return size2;
//	if (size2 == 0) return size1;
//
//	for (int i = 1; i <= size1; ++i)
//	{
//		dst1[0] = i;
//
//		int count = smaller<int>(i, size2);
//		// use formula to fill in the rest of the row
//		for (int j = 1; j <= count; ++j)
//		{
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//			dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
//		}
//
//		std::swap(dst1, dst2);
//		//print_rows();
//	}
//	for (int i = size1; i >= 0; --i)
//	{
//		for (int j = size1 - i + 1; j <= size2; ++j)
//		{
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//			dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
//		}
//
//		std::swap(dst1, dst2);
//		//print_rows();
//	}
//
//	return dst2[size2];
//}
//data_element compute_matrix()
//{
//	int diag_len;
//
//	// upper-left matrix triangle - diagonal size is growing
//	for (diag_len = 1; diag_len < size2; ++diag_len)
//	{
//		int j = 1;
//		for (int i = diag_len; i > 0; --i, ++j)
//		{
//			data_element upper = get(i - 1, j);
//			data_element left = get(i, j - 1);
//			data_element upperleft = get(i - 1, j - 1);
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//
//			set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
//		}
//	}
//	// middle of the mattrix - diagonal is constant
//	diag_len = size2;
//	for (int x = size2; x < size1; ++x)
//	{
//		int i = x;
//		for (int j = 1; j <= diag_len; ++j, --i)
//		{
//			data_element upper = get(i - 1, j);
//			data_element left = get(i, j - 1);
//			data_element upperleft = get(i - 1, j - 1);
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//
//			set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
//		}
//	}
//	// bottom-right matrix triangle - diagonal size is 
//	for (diag_len = size2 - 1; diag_len >= 0; --diag_len)
//	{
//		int i = size1;
//		for (int j = size2 - diag_len; j <= size2; ++j, --i)
//		{
//			data_element upper = get(i - 1, j);
//			data_element left = get(i, j - 1);
//			data_element upperleft = get(i - 1, j - 1);
//			char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
//
//			set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
//		}
//	}
//
//	return get(size1, size2);
//}
//
//data_element get(std::size_t i, std::size_t j)
//{
//	return mat[i * width + j];
//}
//void set(std::size_t i, std::size_t j, data_element val)
//{
//	mat[i * width + j] = val;
//}
//void print_matrix()
//{
//	std::cout << std::endl;
//	for (std::size_t i = 0; i <= size1; ++i)
//	{
//		for (std::size_t j = 0; j <= size2; ++j)
//		{
//			std::cout << get(i, j) << " ";
//		}
//		std::cout << std::endl;
//	}
//}