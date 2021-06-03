
#pragma once

#include "TypeToken.h"

namespace parser::type {

	class StringToken : public TypeToken {
	public:
		[[nodiscard]] std::string getStringId() const override;
		[[nodiscard]] db::COLUMN_T getColumnType() const override;
	};

}
