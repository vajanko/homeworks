void print_matrix(const std::vector<std::vector<size_type>> &matrix)
{
	for (auto& row : matrix)
	{
		for (auto& item : row)
		{
			std::cout << item << " ";
		}
		std::cout << std::endl;
	}
}


void calculate_diagonal(size_type base, size_type block_count, size_type block_size, const char*s, const char *t, size_type *dst1, size_type *dst2, std::vector<std::vector<size_type>> &matrix)
{
	// in this step we have "block_count" block on the diagonale
	// we will start with the block most up and right and follow down and left

	for (size_type block_index = 1; block_index <= block_count; ++block_index)
	{
		// start (up-)right and move (down-)left  -> first part is the block index, multipling by block_size
		// we get the actual position
		size_type h_start = (block_index + base) * block_size + 1;
		// just add block_size to get at the end of current block
		size_type h_end = h_start + block_size;

		// vertical start
		size_type v_start = (block_count - block_index + 1 + base) * block_size + 1;
		size_type v_end = v_start + block_size;

		//size_type h_start = (block_index - 1) * block_size + 1;
		//size_type v_start = (block_count - block_index) * block_size + 1;
		// ---
		//size_type h_start = (block_index + base) * block_size + 1;
		//size_type v_start = (block_count - block_index + 1 + base) * block_size + 1;

		// pointers to the current block
		size_type *blk1 = dst1;
		size_type *blk2 = dst2;

		// the same algorithm as for serial version, but only on this particular chunk of data
		for (size_type i = h_start; i < h_end; ++i)			// block_size iterations
		{
			// only if we are at the left side of the matrix
			if (v_start == 1)
			{
				blk1[0] = i;
				matrix[i][0] = i;
			}

			for (size_type j = v_start; j < v_end; ++j)		// block_size iterations
			{
				// this is calculation for one block

				char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

				size_type upper = blk2[j];
				size_type left = blk1[j - 1];
				size_type upperleft = blk2[j - 1];

				/*size_type upper = matrix[i - 1][j];
				size_type left = matrix[i][j - 1];
				size_type upperleft = matrix[i - 1][j - 1];*/

				size_type total = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
				blk1[j] = total;
				matrix[i][j] = blk1[j];
			}

			std::swap(blk1, blk2);
		}
	}
}

size_type levenshtein_distance(const char *s, size_t s_size, const char *t, size_t t_size)
{
	// debug
	std::vector<std::vector<size_type>> matrix(s_size + 1, std::vector<size_type>(t_size + 1, 0));

	// degenerate cases
	if (s_size == 0) return t_size;
	if (t_size == 0) return s_size;

	// create two work vectors of integer distances
	size_t vec_size = t_size + 1;

	size_type *dst1 = new size_type[vec_size];
	size_type *dst2 = new size_type[vec_size];

	// initialize v0 (the previous row of distances)
	// this row is A[0][i]: edit distance for an empty s
	// the distance is just the number of characters to delete from t
	for (int i = 0; i < vec_size; ++i)
	{
		dst2[i] = i;	// dst2: 0, 1, 2, 3, 4, ...
		//dst1[i] = 0;	// dst1: 0, 0, 0, 0, 0, ...
		matrix[0][i] = i;
	}


	// suppose we are doing the calculation in a matrix. we divide this matrix to equal 
	// blocks each of block_size

	size_type block_size = 2;
	// width of matrix divided into blocks
	size_type total_blocks = t_size / block_size;

	// TEST
	for (size_type block_count = 1; block_count <= total_blocks; ++block_count)
	{
		calculate_diagonal(-1, block_count, block_size, s, t, dst1, dst2, matrix);
	}
	for (size_type block_count = total_blocks - 1; block_count > 0; --block_count)
	{
		size_type base = total_blocks - 1 - block_count;
		calculate_diagonal(base, block_count, block_size, s, t, dst1, dst2, matrix);
	}
	std::cout << "parallel matrix" << std::endl;
	print_matrix(matrix);
	return dst2[t_size];
	// TEST

	// first part: in each step one more block is added
	// run calculation for 1, 2, 3, ... blocks, block_count is the number of blocks we can process in parallel
	for (size_type block_count = 1; block_count <= total_blocks; ++block_count)
	{
		// in this step we have "block_count" block on the diagonale
		// we will start with the block most up and right and follow down and left

		for (size_type block_index = 1; block_index <= block_count; ++block_index)
		{
			// start (up-)right and move (down-)left  -> first part is the block index, multipling by block_size
			// we get the actual position
			size_type h_start = (block_index - 1) * block_size + 1;
			// just add block_size to get at the end of current block
			size_type h_end = h_start + block_size;

			// vertical start
			size_type v_start = (block_count - block_index) * block_size + 1;
			size_type v_end = v_start + block_size;

			// pointers to the current block
			size_type *blk1 = dst1;
			size_type *blk2 = dst2;

			// the same algorithm as for serial version, but only on this particular chunk of data
			for (size_type i = h_start; i < h_end; ++i)			// block_size iterations
			{
				// only if we are at the left side of the matrix
				if (v_start == 1)
				{
					blk1[0] = i;
					matrix[i][0] = i;
				}

				for (size_type j = v_start; j < v_end; ++j)		// block_size iterations
				{
					// this is calculation for one block

					char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

					size_type upper = blk2[j];
					size_type left = blk1[j - 1];
					size_type upperleft = blk2[j - 1];

					/*size_type upper = matrix[i - 1][j];
					size_type left = matrix[i][j - 1];
					size_type upperleft = matrix[i - 1][j - 1];*/

					blk1[j] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
					matrix[i][j] = blk1[j];
				}

				std::swap(blk1, blk2);
			}
		}
	}

	// constant block_count ...

	for (size_type block_count = total_blocks - 1; block_count > 0; --block_count)
	{
		for (size_type block_index = 1; block_index <= block_count; ++block_index)
		{
			size_type base = total_blocks - 1 - block_count;

			// start (up-)right and move (down-)left  -> first part is the block index, multipling by block_size
			// we get the actual position
			size_type h_start = (block_index + base) * block_size + 1;
			// just add block_size to get at the end of current block
			size_type h_end = h_start + block_size;

			// vertical start
			size_type v_start = (block_count - block_index + 1 + base) * block_size + 1;
			size_type v_end = v_start + block_size;

			// pointers to the current block
			size_type *blk1 = dst1;
			size_type *blk2 = dst2;

			// the same algorithm as for serial version, but only on this particular chunk of data
			for (size_type i = h_start; i < h_end; ++i)			// block_size iterations
			{
				// only if we are at the left side of the matrix
				if (v_start == 1)
				{
					blk1[0] = i;
					matrix[i][0] = i;
				}


				for (size_type j = v_start; j < v_end; ++j)		// block_size iterations
				{
					// this is calculation for one block

					char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

					/*size_type upper = blk2[j];
					size_type left = blk1[j - 1];
					size_type upperleft = blk2[j - 1];*/

					size_type upper = matrix[i - 1][j];
					size_type left = matrix[i][j - 1];
					size_type upperleft = matrix[i - 1][j - 1];

					blk1[j] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
					matrix[i][j] = blk1[j];
				}

				std::swap(blk1, blk2);
			}
		}
	}

	//std::cout << "parallel matrix" << std::endl;
	//print_matrix(matrix);

	return dst2[t_size];
}